using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace NeuralNetwork1
{
    public class StudentNetwork : BaseNetwork
    {
        private int[] structure;

        private double[] neurons;
        private double[] deltas;
        private double[] weights;

        private int[] neuronOffsets;
        private int[] deltaOffsets;
        private int[] weightOffsets;

        private Random rand = new Random();
        private Stopwatch stopwatch = new Stopwatch();

        private const double LearningRate = 0.2;

        public StudentNetwork(int[] structure)
        {
            this.structure = structure;

            neuronOffsets = new int[structure.Length];
            deltaOffsets = new int[structure.Length];
            weightOffsets = new int[structure.Length - 1];

            int nOffset = 0;
            int dOffset = 0;
            int wOffset = 0;

            for (int l = 0; l < structure.Length; l++)
            {
                neuronOffsets[l] = nOffset;
                deltaOffsets[l] = dOffset;

                nOffset += structure[l] + (l < structure.Length - 1 ? 1 : 0);
                dOffset += structure[l];

                if (l < structure.Length - 1)
                {
                    weightOffsets[l] = wOffset;
                    wOffset += (structure[l] + 1) * structure[l + 1];
                }
            }

            neurons = new double[nOffset];
            deltas = new double[dOffset];
            weights = new double[wOffset];

            for (int i = 0; i < weights.Length; i++)
                weights[i] = (rand.NextDouble() - 0.5) * 0.6;
        }
        private static double Activate(double x)
        {
            return 1.0 / (1.0 + System.Math.Exp(-x));
        }

        protected override double[] Compute(double[] input)
        {
            int inOff = neuronOffsets[0];
            for (int i = 0; i < input.Length; i++)
                neurons[inOff + i] = input[i];

            neurons[inOff + structure[0]] = 1.0;

            for (int l = 0; l < structure.Length - 1; l++)
            {
                int curOff = neuronOffsets[l];
                int nextOff = neuronOffsets[l + 1];
                int wOff = weightOffsets[l];

                int inputs = structure[l] + 1;
                int outputs = structure[l + 1];

                Parallel.For(0, outputs, j =>
                {
                    double sum = 0;
                    for (int i = 0; i < inputs; i++)
                        sum += weights[wOff + j * inputs + i] * neurons[curOff + i];

                    neurons[nextOff + j] = Activate(sum);
                });

                if (l + 1 < structure.Length - 1)
                    neurons[nextOff + outputs] = 1.0;
            }

            int last = structure.Length - 1;
            double[] output = new double[structure[last]];
            Array.Copy(neurons, neuronOffsets[last], output, 0, output.Length);
            return output;
        }

        public override int Train(Sample sample, double acceptableError, bool parallel)
        {
            int iter = 0;
            double error;

            do
            {
                iter++;
                error = TrainSample(sample, parallel);
            }
            while (error > acceptableError);

            return iter;
        }

        private static double Derivative(double y)
        {
            return y * (1.0 - y);
        }

        private double TrainSample(Sample sample, bool parallel)
        {
            double[] output = Compute(sample.input);
            int last = structure.Length - 1;

            int nOff = neuronOffsets[last];
            int dOff = deltaOffsets[last];

            for (int i = 0; i < structure[last]; i++)
            {
                double e = sample.Output[i] - output[i];
                deltas[dOff + i] = e * Derivative(neurons[nOff + i]);
            }

            for (int l = last - 1; l > 0; l--)
            {
                int curDO = deltaOffsets[l];
                int nextDO = deltaOffsets[l + 1];
                int curNO = neuronOffsets[l];
                int wOff = weightOffsets[l];

                int inputs = structure[l];
                int outputs = structure[l + 1];

                for (int i = 0; i < inputs; i++)
                {
                    double sum = 0;
                    for (int j = 0; j < outputs; j++)
                        sum += deltas[nextDO + j] * weights[wOff + j * (inputs + 1) + i];

                    deltas[curDO + i] = sum * Derivative(neurons[curNO + i]);
                }
            }

            for (int l = 0; l < structure.Length - 1; l++)
            {
                int curNO = neuronOffsets[l];
                int nextDO = deltaOffsets[l + 1];
                int wOff = weightOffsets[l];

                int inputs = structure[l] + 1;
                int outputs = structure[l + 1];

                if (parallel)
                {
                    Parallel.For(0, outputs, j =>
                    {
                        int wBase = wOff + j * inputs;
                        double delta = deltas[nextDO + j];

                        for (int i = 0; i < inputs; i++)
                        {
                            weights[wBase + i] +=
                                LearningRate * delta * neurons[curNO + i];
                        }
                    });
                }
                else
                {
                    for (int j = 0; j < outputs; j++)
                        for (int i = 0; i < inputs; i++)
                            weights[wOff + j * inputs + i] +=
                                LearningRate * deltas[nextDO + j] * neurons[curNO + i];
                }
            }

            double error = 0;
            for (int i = 0; i < structure[last]; i++)
            {
                double d = sample.Output[i] - output[i];
                error += d * d;
            }

            return error;
        }

        public override double TrainOnDataSet(SamplesSet samplesSet, int epochsCount, double acceptableError, bool parallel)
        {
            stopwatch.Restart();
            double error = double.MaxValue;
            int epoch = 0;

            while (epoch < epochsCount && error > acceptableError)
            {
                epoch++;
                error = 0;

                foreach (Sample sample in samplesSet)
                    error += TrainSample(sample, parallel);

                OnTrainProgress((double)epoch / epochsCount, error, stopwatch.Elapsed);
            }

            OnTrainProgress(1.0, error, stopwatch.Elapsed);

            stopwatch.Stop();
            return error;
        }
    }
}
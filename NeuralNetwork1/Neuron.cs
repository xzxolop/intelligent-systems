using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NeuralNetwork1
{
    public class Neuron
    {

        // или более правильно:
        short signal;

        public void setSignal(short x)
        {
            signal = x;
        }

        public double Sigmoid(double x)
        {
            return 1.0 / (1.0 + Math.Exp(-x));
        }

        public double activationFunction(double x)
        {
            return Sigmoid(x);
        }
    }
}

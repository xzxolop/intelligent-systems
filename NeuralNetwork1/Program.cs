//#define EnableAccordNet

using Accord.Math.Random;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace NeuralNetwork1
{
    static class Program
    {
        /// <summary>
        /// Главная точка входа для приложения.
        /// </summary>
        [STAThread]
        static void Main()
        {

#if EnableAccordNet
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new NeuralNetworksStand(new Dictionary<string, Func<int[], BaseNetwork>>
            {
                // Тут можно добавить свои нейросети
                {"Accord.Net Perseptron", structure => new AccordNet(structure)},
                {"Студентческий персептрон", structure => new StudentNetwork(structure)},
            }));
#endif
            GenerateImage generator = new GenerateImage();
            SamplesSet samplesSet = new SamplesSet();

            // Создаём 100 различных фигур
            for (int i = 0; i < 10; i++)
            {
                // Генерация случайной фигуры
                Sample sample = generator.GenerateFigure();
                samplesSet.AddSample(sample);
                Console.WriteLine($"Фигура {i + 1}: {sample.actualClass}");
            }

            Sample s1 = samplesSet[0];
            
            Console.WriteLine(s1);
        }
    }
}
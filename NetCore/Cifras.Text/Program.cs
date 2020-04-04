using System;

namespace Cifras.Text
{
    class Program
    {
        static void Main(string[] args)
        {
            var pGenerado = new Cifras.Core.Generador.GeneradorEnunciadoTVE();
            var pEnunciado = pGenerado.Generar();

            Console.WriteLine("Hello World!");
        }
    }
}

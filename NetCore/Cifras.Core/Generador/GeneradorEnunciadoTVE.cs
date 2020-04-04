using Cifras.Core.Util;
using Cifras.Core.Extensions;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Cifras.Core.Generador
{
    public class GeneradorEnunciadoTVE : IGeneradorEnunciadoElegirGrupo
    {
        protected readonly static int[][] mGruposInitial =
            new int[][]
        {
            new int[] { 1,2,3,4,5,6 },
            new int[] { 7,8,9,1,2,3 },
            new int[] { 4,5,6,7,8,9 },
            new int[] { 10,10,25,50,75,100 }
        };

        private RandomRNG mRandom = new RandomRNG();
        private int[][] mGrupos;
        private Enunciado mEnunciado = new Enunciado { Pistas = new int[6] };

        public GeneradorEnunciadoTVE()
        {
            GenerarGrupos();
        }

        public int GrupoInicial => 1;

        public int GrupoFinal => 1;

        public int[] LongGrupos { get; set; } = new[] { 6, 6, 6, 6 };

        public int IdxPistaAct { get; private set; } = 0;

        public Enunciado Generar()
        {
            while (IdxPistaAct < mEnunciado.Pistas.Length)
                SetGrupoCPU();

            return mEnunciado;
        }

        public void SetGrupo(int argGrupo)
        {
            if (IdxPistaAct >= mEnunciado.Pistas.Length || argGrupo > mGrupos.Length)
                return;

            var pIdxGrupo = argGrupo - 1;
            var pIdx = IdxPistaAct++;

            mEnunciado.Pistas[pIdx] = mGrupos[pIdxGrupo][--LongGrupos[pIdxGrupo]];
        }

        public void SetGrupoCPU()
        {
            SetGrupo(mRandom.Next(mGrupos.Length) + 1);
        }

        private void GenerarGrupos()
        {
            mGrupos = (from g in mGruposInitial select g.Shuffle().ToArray()).ToArray();
        }
    }
}

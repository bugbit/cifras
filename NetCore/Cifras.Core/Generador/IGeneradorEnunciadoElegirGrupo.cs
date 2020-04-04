using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;

namespace Cifras.Core.Generador
{
    public interface IGeneradorEnunciadoElegirGrupo : IGeneradorEnunciado
    {
        int GrupoInicial { get; }
        int GrupoFinal { get; }
        int[] LongGrupos { get; }
        int IdxPistaAct { get; }
        void SetGrupo(int argGrupo);
        void SetGrupoCPU();
    }
}

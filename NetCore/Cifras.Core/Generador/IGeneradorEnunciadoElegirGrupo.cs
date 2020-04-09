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
        bool SeHaAcabadoDeEligir { get; }
        void SetGrupo(int argGrupo);
        int SetGrupoCPU();
    }
}

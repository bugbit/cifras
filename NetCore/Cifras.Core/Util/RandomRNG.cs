using System;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Text;

namespace Cifras.Core.Util
{
    public class RandomRNG
    {
        private static RNGCryptoServiceProvider mRNG = new RNGCryptoServiceProvider();

        public int Next(int argMax)
        {
            var pData = new byte[4];

            mRNG.GetBytes(pData); // thread-safe; no lock necessary

            return (int)(BitConverter.ToUInt32(pData, 0) % argMax);
        }
    }
}

using System;
using System.Collections.Generic;
using System.Text;

namespace Cifras.Core.Util
{
    public class HelperLinq
    {
		public static void Swap<T>(ref T argFirst, ref T argSecond)
		{
			var pTmp = argFirst;

			argFirst = argSecond;
			argSecond = pTmp;
		}
	}
}

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CloudGen
{
    public static class ExtensionMethods
    {
        public static int IndexOfMax<TItem, TValue>(this IEnumerable<TItem> items, Func<TItem, TValue> value) where TValue : IComparable<TValue>
        {
            bool first = true;
            int maxIndex = -1;
            TValue maxValue = default(TValue);
            int index = 0;
            foreach (TItem item in items)
            {
                if (first)
                {
                    maxIndex = 0;
                    maxValue = value(item);
                    first = false;
                }
                else
                {
                    TValue itemValue = value(item);
                    if (itemValue.CompareTo(maxValue) > 0)
                    {
                        maxIndex = index;
                        maxValue = itemValue;
                    }
                }
                index++;
            }
            return maxIndex;
        }
    }
}

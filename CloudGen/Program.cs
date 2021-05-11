using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace CloudGen
{
    class Program
    {
        static void Main(string[] args)
        {
            var lines = new (Vector3, Vector3, int)[]
            {
				// Note: these were the values used, but the left and right bars should actually be below the front & back bars
                (new Vector3(-7f, 4.75f, -2.375f), new Vector3(7f, 4.75f, -2.375f), 15), // Front bar
                (new Vector3(-7f, 4.75f, 2.375f), new Vector3(7f, 4.75f, 2.375f), 15), // Back bar
                (new Vector3(-7f, 4.75f, -5.5f), new Vector3(-7f, 4.75f, 5.5f), 12), // Left bar
                (new Vector3(7f, 4.75f, -5.5f), new Vector3(7f, 4.75f, 5.5f), 12), // Right bar
                (new Vector3(5.125f, 5.125f, 5.375f), new Vector3(5.125f, 0, 5.375f), 6), // Upright
                (new Vector3(-4f, 0, 2.375f), new Vector3(4.125f, 0, 5.375f), 10), // Left upright connector
                (new Vector3(4f, 0, 2.375f), new Vector3(5.125f, 0, 4.375f), 10), // Right upright connector
                (new Vector3(-4f, 0, -2.375f), new Vector3(4f, 0, -2.375f), 9), // Reservoir front bottom
            };
            var pointList = new List<Vector3>();
            foreach ((Vector3 p1, Vector3 p2, int n) in lines)
            {
                for (int t=0; t < n; t++)
                {
                    pointList.Add(p1 + (p2 - p1) * t / (n - 1));
                }
            }
            Vector3[] points = pointList.ToArray();

            var min = new Vector3(-8.5f, -12f, -6.5f);
            var max = new Vector3(8.5f, 8f, 6f);

            var r = new Random();

            Cloud final;
            while (true)
            {
                var newCloud = new Cloud();
                newCloud.Encompass(points, r, min, max);

                var refined = Refine(newCloud, points, r, min, max, 5, 5);

                if (refined.Volume() < 1500)
                {
                    final = refined;
                    break;
                }
            }

            Console.WriteLine(final.Volume());
            Console.WriteLine(final.AsPOVRay());
        }

        static Cloud Refine(Cloud initial, Vector3[] points, Random r, Vector3 min, Vector3 max, int iterationsPerLevel, int levels)
        {
            if (levels == 0)
            {
                return initial;
            }

            float lowestVolume = initial.Volume();
            Cloud bestResult = initial;
            //Console.WriteLine(string.Format("Level {0} baseline: {1}", levels, lowestVolume));
            for (int i = 0; i < iterationsPerLevel; i++)
            {
                Cloud iterationResult = new Cloud(initial);
                iterationResult.RemoveLargestBall();
                iterationResult.Encompass(points, r, min, max);
                float iterationVolume = iterationResult.Volume();
                //Console.WriteLine(string.Format("Iteration {0} of {1}, level {2}: {3}", i, iterationsPerLevel, levels, iterationVolume));
                if (iterationVolume < lowestVolume)
                {
                    bestResult = iterationResult;
                    lowestVolume = iterationVolume;
                }
            }

            return Refine(bestResult, points, r, min, max, iterationsPerLevel, levels - 1);
        }
    }
}

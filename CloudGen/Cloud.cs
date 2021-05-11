using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;

namespace CloudGen
{
    struct Sphere
    {
        public Vector3 Center;
        public float Radius;

        public bool Contains(Sphere other)
        {
            return (Center - other.Center).Length() < other.Radius - Radius;
        }

        public bool Contains(Vector3 point)
        {
            return (point - Center).Length() <= Radius;
        }

        public bool ContainsAny(IEnumerable<Vector3> points)
        {
            Sphere s = this;
            return points.Any(p => s.Contains(p));
        }

        public override string ToString()
        {
            return Radius.ToString() + " @ " + Center.ToString();
        }
    }

    struct BallGenerator
    {
        public Vector3 Mu;
        public Vector3 Sigma;
        public float Lambda;
        public float k;

        public Sphere MakeBall(Random r)
        {
            return new Sphere() { Center = new Vector3(RandNormal(r, Mu.X, Sigma.X), RandNormal(r, Mu.Y, Sigma.Y), RandNormal(r, Mu.Z, Sigma.Z)), Radius = RandWeibull(r, Lambda, k) };
        }

        private static float RandNormal(Random r, float mu, float sigma)
        {
            double u1 = 1.0 - r.NextDouble();
            double u2 = 1.0 - r.NextDouble();
            double randStdNormal = Math.Sqrt(-2.0 * Math.Log(u1)) * Math.Sin(2.0 * Math.PI * u2); //random normal(0,1)
            double randNormal = mu + sigma * randStdNormal; //random normal(mean,stdDev^2)
            return (float)randNormal;
        }

        private static float RandWeibull(Random r, float lambda, float k)
        {
            return (float)(lambda * Math.Pow(-Math.Log(1.0 - r.NextDouble()), 1.0 / k));
        }
    }

    class Cloud
    {
        public List<Sphere> Balls = new List<Sphere>();

        public Cloud(Cloud other = null)
        {
            if (other != null)
            {
                Balls.AddRange(other.Balls);
            }
        }

        public bool ContainsAll(IEnumerable<Vector3> points)
        {
            return points.All(p => Contains(p));
        }

        public int Contains(IEnumerable<Vector3> points)
        {
            return points.Select(p => Contains(p) ? 1 : 0).Sum();
        }

        public bool Contains(Vector3 point)
        {
            return Balls.Any(b => b.Contains(point));
        }

        public (Vector3, Vector3) BoundingBox()
        {
            Vector3 min = new Vector3(float.PositiveInfinity, float.PositiveInfinity, float.PositiveInfinity);
            Vector3 max = new Vector3(float.NegativeInfinity, float.NegativeInfinity, float.NegativeInfinity);
            foreach (Sphere ball in Balls)
            {
                min = new Vector3(Math.Min(min.X, ball.Center.X - ball.Radius), Math.Min(min.Y, ball.Center.Y - ball.Radius), Math.Min(min.Z, ball.Center.Z - ball.Radius));
                max = new Vector3(Math.Max(max.X, ball.Center.X + ball.Radius), Math.Max(max.Y, ball.Center.Y + ball.Radius), Math.Max(max.Z, ball.Center.Z + ball.Radius));
            }
            return (min, max);
        }

        public float Volume(int resolution = 1000)
        {
            (Vector3 bbMin, Vector3 bbMax) = BoundingBox();
            Vector3 bbSize = bbMax - bbMin;
            float bbVolume = bbSize.X * bbSize.Y * bbSize.Z;
            float q = (float)Math.Pow(bbVolume / resolution, 1.0 / 3.0);
            int nx = (int)Math.Round(bbSize.X / q);
            int ny = (int)Math.Round(bbSize.Y / q);
            int nz = (int)Math.Round(bbSize.Z / q);
            float x0 = (bbMin.X + bbMax.X) / 2 - q * (nx - 1) / 2;
            float y0 = (bbMin.Y + bbMax.Y) / 2 - q * (ny - 1) / 2;
            float z0 = (bbMin.Z + bbMax.Z) / 2 - q * (nz - 1) / 2;
            var points = new Vector3[nx * ny * nz];
            int i = 0;
            for (int z = 0; z < nz; z++)
            {
                for (int y = 0; y < ny; y++)
                {
                    for (int x = 0; x < nx; x++)
                    {
                        points[i++] = new Vector3(x0 + x * q, y0 + y * q, z0 + z * q);
                    }
                }
            }
            return bbVolume * Contains(points) / points.Length;
        }

        private static Vector3 SquareComponents(Vector3 v)
        {
            return new Vector3(v.X * v.X, v.Y * v.Y, v.Z * v.Z);
        }

        private static Vector3 SqrtComponents(Vector3 v)
        {
            return new Vector3((float)Math.Sqrt(v.X), (float)Math.Sqrt(v.Y), (float)Math.Sqrt(v.Z));
        }

        public void Encompass(Vector3[] points, Random r, Vector3 min, Vector3 max)
        {
            while (true)
            {
                Vector3[] outsidePoints = points.Where(p => !Contains(p)).ToArray();
                if (outsidePoints.Length == 0)
                {
                    break;
                }
                Vector3 center = outsidePoints.Aggregate((p1, p2) => p1 + p2) / outsidePoints.Length;
                Vector3 rms = SqrtComponents(outsidePoints.Select(p => SquareComponents(p - center)).Aggregate((p1, p2) => p1 + p2) / outsidePoints.Length);
                var g = new BallGenerator() { Mu = center, Sigma = new Vector3(Math.Max(rms.X, 1), Math.Max(rms.Y, 1), Math.Max(rms.Z, 1)), Lambda = Math.Max(rms.Length() / 0.886227f, 1), k = 2 };
                while (true)
                {
                    Sphere newBall = g.MakeBall(r);
                    bool valid = true;

                    Vector3 dMin = newBall.Center - min;
                    if (dMin.X < newBall.Radius || dMin.Y < newBall.Radius || dMin.Z < newBall.Radius)
                    {
                        valid = false;
                        continue;
                    }
                    Vector3 dMax = max - newBall.Center;
                    if (dMax.X < newBall.Radius || dMax.Y < newBall.Radius || dMax.Z < newBall.Radius)
                    {
                        valid = false;
                        continue;
                    }

                    foreach (Sphere oldBall in Balls)
                    {
                        if (newBall.Contains(oldBall))
                        {
                            valid = false;
                            break;
                        }
                    }

                    if (valid && newBall.ContainsAny(outsidePoints))
                    {
                        Balls.Add(newBall);
                        break;
                    }
                }
            }
        }

        public void RemoveLargestBall()
        {
            Balls.RemoveAt(Enumerable.Range(0, Balls.Count).IndexOfMax(i => Balls[i].Radius));
        }

        public string AsPOVRay()
        {
            var sb = new StringBuilder();
            foreach (Sphere ball in Balls)
            {
                sb.AppendLine(string.Format("        sphere {{ <{0:f2},{1:f2},{2:f2}>, {3:f2}, 2.0 }}", ball.Center.X, ball.Center.Y, ball.Center.Z, ball.Radius * Math.Sqrt(2 + Math.Sqrt(2))));
            }
            return sb.ToString();
        }
    }
}

using System;
using System.Diagnostics;
using System.Threading;

class Program
{
    static void Main()
    {
        int totalPoints = 1_000_000;
        int[] threadCounts = { 1, 2, 4, 8, 16, 32, 64 };

        foreach (int threadsCount in threadCounts)
        {
            double pi = RunMonteCarlo(totalPoints, threadsCount, out long ms);
            Console.WriteLine($"Threads: {threadsCount,-2} | pi = {pi:F6} | Time: {ms} ms");
        }
        Console.ReadLine();
    }

    static double RunMonteCarlo(int totalPoints, int threadsCount, out long elapsedMs)
    {
        int pointsPerThread = totalPoints / threadsCount;
        int[] localResults = new int[threadsCount];
        Thread[] threads = new Thread[threadsCount];
        Stopwatch sw = Stopwatch.StartNew();

        for (int i = 0; i < threadsCount; i++)
        {
            int idx = i;
            threads[i] = new Thread(() =>
            {
                int inside = 0;
                Random rand = new Random(unchecked(Environment.TickCount * 31 + idx * 997));
                for (int j = 0; j < pointsPerThread; j++)
                {
                    double x = rand.NextDouble();
                    double y = rand.NextDouble();
                    if (x * x + y * y <= 1) inside++;
                }
                localResults[idx] = inside;
            });
            threads[i].Start();
        }

        foreach (var t in threads)
            t.Join();

        sw.Stop();

        int totalInside = 0;
        for (int i = 0; i < threadsCount; i++)
            totalInside += localResults[i];

        double pi = 4.0 * totalInside / totalPoints;
        elapsedMs = sw.ElapsedMilliseconds;
        return pi;
    }
}

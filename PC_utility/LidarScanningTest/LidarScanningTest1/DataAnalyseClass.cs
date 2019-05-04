using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LidarScanningTest1
{
    class DataAnalyseClass
    {
        FixedSizedQueue<double> FIFO;

        public double average = 0;
        public double min_max = 0;

        public DataAnalyseClass()
        {
            FIFO = new FixedSizedQueue<double>();
            FIFO.Limit = 5;
        }

        public void AddDataPoint(double value)
        {
            //small distancec are not used in analyse
            if (value < 0.05)
                return;

            FIFO.Enqueue(value);

            double[] data_buf = FIFO.q.ToArray();

            if (data_buf.Length < 5)
                return;

            double tmp = 0;

            for (int i = 0; i < data_buf.Length; i++)
            {
                tmp += data_buf[i];
            }
            average = tmp / data_buf.Length;

            double min_dist = data_buf.Min();
            double max_dist = data_buf.Max();

            min_max = max_dist - min_dist;
        }

        public void SetLength(int length)
        {
            if (length < 1)
                return;

            FIFO.Limit = length;
        }
    }

    public class FixedSizedQueue<T>
    {
        public ConcurrentQueue<T> q = new ConcurrentQueue<T>();
        private object lockObject = new object();

        public int Limit { get; set; }
        public void Enqueue(T obj)
        {
            q.Enqueue(obj);
            lock (lockObject)
            {
                T overflow;
                while (q.Count > Limit && q.TryDequeue(out overflow)) ;
            }
        }
    }
}

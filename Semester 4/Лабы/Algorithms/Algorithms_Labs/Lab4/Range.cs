namespace Lab4
{
    public class Range
    {
        public int L { get; }
        public int H { get; }
        public int Difference { get; }

        public Range(int l, int h)
        {
            L = l;
            H = h;
            Difference = h - l;
        }
    }
}
using Newtonsoft.Json;

namespace Lab1.Models
{
    public class Aircraft
    {
        public int Id { get; set; }

        public string Name { get; set; }
        public AircraftType Type { get; set; }
        public int Engines { get; set; }
        public int Price { get; set; }

        public override string ToString()
        {
            return JsonConvert.SerializeObject(this);
        }
    }
}
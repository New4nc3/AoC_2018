class Program
{
    //private const string INPUT = @"..\..\..\test.txt";
    private const string INPUT = @"..\..\..\input.txt";

    static void Main()
    {
        var nodeProcessor = new NodeProcessor(INPUT);
        var sumMetadatas = nodeProcessor.Part1();

        Console.WriteLine($"Part 1. Sum of all metadatas: {sumMetadatas}");
        Console.WriteLine("\nPress any key to exit . . .");
        Console.Read();
    }
}

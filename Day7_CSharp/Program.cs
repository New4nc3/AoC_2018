class Program
{
    //private const string INPUT = @"..\..\..\test.txt";
    private const string INPUT = @"..\..\..\input.txt";

    static void Main()
    {
        var nodeProcessor = new NodeProcessor(INPUT);
        nodeProcessor.SolvePart2();

        Console.WriteLine("\nPress any key to exit . . .");
        Console.Read();
    }
}

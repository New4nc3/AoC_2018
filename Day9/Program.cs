class Program
{
    //private const string INPUT = @"..\..\..\test.txt";
    private const string INPUT = @"..\..\..\input.txt";

    static void Main()
    {
        var (Players, LastMarble) = ReadFromFile(INPUT);

        MarbleProcessor marbleProcessor = new MarbleProcessor(Players, LastMarble);
        marbleProcessor.Solve(part2: false);
        marbleProcessor.Solve(part2: true);
    }

    static (int Players, ulong LastMarble) ReadFromFile(string filename)
    {
        string[] inputWords;
        using (StreamReader streamReader = new StreamReader(filename))
            inputWords = streamReader.ReadLine()
                ?.Split(" ") ?? new string[0];

        return (Convert.ToInt32(inputWords[0]), Convert.ToUInt64(inputWords[6]));
    }
}

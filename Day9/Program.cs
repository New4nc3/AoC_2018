class Program
{
    //private const string INPUT = @"..\..\..\test.txt";
    private const string INPUT = @"..\..\..\input.txt";

    static void Main()
    {
        var (Players, LastMarble) = ReadFromFile(INPUT);
        MarbleProcessor marbleProcessor = new MarbleProcessor(Players, LastMarble);
        marbleProcessor.Solve();
    }

    static (int Players, int LastMarble) ReadFromFile(string filename)
    {
        string[] inputWords;
        using (StreamReader streamReader = new StreamReader(filename))
            inputWords = streamReader.ReadLine()
                ?.Split(" ") ?? new string[0];

        return (Convert.ToInt32(inputWords[0]), Convert.ToInt32(inputWords[6]));
    }
}

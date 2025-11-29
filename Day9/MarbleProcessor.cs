class MarbleProcessor
{
    public int PlayersCount { get; }
    public int LastMarbleValue { get; }

    public List<int> Marbles { get; }
    public Dictionary<int, int> Players { get; }


    public MarbleProcessor(int players, int lastMarbleValue)
    {
        (PlayersCount, LastMarbleValue) = (players, lastMarbleValue);

        Marbles = new List<int>();
        Players = new Dictionary<int, int>();
    }

    public void Solve()
    {
        InitMarbles();
        InitPlayers();

        int currentMarbleIndex = 0;
        int currentPlayerIndex = 0;
        int marbleValue = 1;

        while (marbleValue <= LastMarbleValue)
        {
            if (marbleValue % 23 == 0)
            {
                Players[currentPlayerIndex] += marbleValue;

                var indexToRemoveAt = GetSeventhCounterClockwiseIndexFor(currentMarbleIndex);
                Players[currentPlayerIndex] += Marbles[indexToRemoveAt];
                Marbles.RemoveAt(indexToRemoveAt);

                currentMarbleIndex = indexToRemoveAt;
                if (currentMarbleIndex == Marbles.Count)
                    currentMarbleIndex = 0;
            }
            else
            {
                var firstClockwiseIndex = GetNextClockwiseIndexFor(currentMarbleIndex);
                var secondClockwiseIndex = GetNextClockwiseIndexFor(firstClockwiseIndex);

                if (firstClockwiseIndex < secondClockwiseIndex)
                {
                    currentMarbleIndex = secondClockwiseIndex;
                    Marbles.Insert(currentMarbleIndex, marbleValue);
                }
                else
                {
                    Marbles.Add(marbleValue);
                    currentMarbleIndex = Marbles.Count - 1;
                }
            }

            ++currentPlayerIndex;
            if (currentPlayerIndex >= PlayersCount)
                currentPlayerIndex = 0;

            ++marbleValue;
        }

        Console.WriteLine($"Part 1. Max score: {Players.Values.Max()}");
    }

    private void InitMarbles()
    {
        Marbles.Clear();
        Marbles.Add(0);
    }

    private void InitPlayers()
    {
        Players.Clear();

        for (var i = 0; i < PlayersCount; ++i)
            Players.Add(i, 0);
    }

    private int GetNextClockwiseIndexFor(int currentIndex)
    {
        ++currentIndex;

        if (currentIndex >= Marbles.Count)
            return 0;

        return currentIndex;
    }

    private int GetSeventhCounterClockwiseIndexFor(int currentIndex)
    {
        currentIndex -= 7;

        if (currentIndex < 0)
            return Marbles.Count + currentIndex;

        return currentIndex;
    }
}

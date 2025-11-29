class MarbleProcessor
{
    public int PlayersCount { get; }
    public ulong LastMarbleValue { get; private set; }

    public List<ulong> Marbles { get; }
    public Dictionary<int, ulong> Players { get; }

    public MarbleProcessor(int players, ulong lastMarbleValue)
    {
        (PlayersCount, LastMarbleValue) = (players, lastMarbleValue);

        Marbles = new List<ulong>();
        Players = new Dictionary<int, ulong>();
    }

    public void Solve(bool part2)
    {
        InitMarbles();
        InitPlayers();

        if (part2)
            LastMarbleValue *= 100;

        int currentMarbleIndex = 0;
        int currentPlayerIndex = 0;
        ulong marbleValue = 1;

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

        Console.WriteLine($"Part {(part2 ? "2" : "1")}. Max score: {Players.Values.Max()}");
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

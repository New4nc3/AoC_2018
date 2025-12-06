class MarbleProcessor
{
    public int PlayersCount { get; }
    public ulong LastMarbleValue { get; private set; }

    public LinkedList<ulong> Marbles { get; }
    public Dictionary<int, ulong> Players { get; }

    public MarbleProcessor(int players, ulong lastMarbleValue)
    {
        (PlayersCount, LastMarbleValue) = (players, lastMarbleValue);

        Marbles = new LinkedList<ulong>();
        Players = new Dictionary<int, ulong>();
    }

    public void Solve(bool part2)
    {
        InitMarbles();
        InitPlayers();

        if (part2)
            LastMarbleValue *= 100;

        var currentMarble = Marbles.First;
        int currentPlayerIndex = 0;
        ulong marbleValue = 1;

        while (marbleValue <= LastMarbleValue)
        {
            if (marbleValue % 23 == 0)
            {
                Players[currentPlayerIndex] += marbleValue;

                for (var i = 0; i < 7; ++i)
                {
                    currentMarble = currentMarble.Previous;
                    if (currentMarble == null)
                        currentMarble = Marbles.Last;
                }

                var valueToReference = currentMarble.Next;
                Players[currentPlayerIndex] += currentMarble.Value;
                Marbles.Remove(currentMarble);
                currentMarble = valueToReference;
            }
            else
            {
                currentMarble = currentMarble.Next;
                if (currentMarble == null)
                    currentMarble = Marbles.First;

                Marbles.AddAfter(currentMarble, marbleValue);
                currentMarble = currentMarble.Next;
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
        Marbles.AddFirst(0);
    }

    private void InitPlayers()
    {
        Players.Clear();

        for (var i = 0; i < PlayersCount; ++i)
            Players.Add(i, 0);
    }
}

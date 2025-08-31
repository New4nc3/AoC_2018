class Worker
{
    private static readonly int LETTER_SHIFT = 64;
    private static readonly int SECONDS_ADD = 60;
    //private static readonly int SECONDS_ADD = 0;

    public int BusyTime { get; private set; }
    public int TotalUpTime { get; private set; } = 0;
    public Node? Node { get; private set; }

    public bool IsBusy => Node != null;
    public bool IsReadyToLaunch => BusyTime >= TimeNeeded;
    public int TimeNeeded => Node == null ? 0 : Node.Name - LETTER_SHIFT + SECONDS_ADD;

    public void SetNode(Node node)
    {
        Node = node;
        BusyTime = 0;

        Node.SetAsReservedByWorker();
    }

    public void ReleaseNode()
    {
        Node?.ReleaseAsReservedByWorker();

        Node = null;
        BusyTime = 0;
    }

    public void AddBusyTime(int seconds)
    {
        BusyTime += seconds;
        AddUpTime(seconds);
    }

    public void AddUpTime(int seconds) =>
        TotalUpTime += seconds;

    public override string ToString() =>
        Node == null ? "FREE" : $"{Node.Name} {BusyTime} {TimeNeeded}";
}

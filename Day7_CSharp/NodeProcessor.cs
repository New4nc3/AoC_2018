class NodeProcessor
{
    private string _filename;
    private Dictionary<char, Node> _nodeMap;

    public NodeProcessor(string filename)
    {
        _filename = filename;
        _nodeMap = new Dictionary<char, Node>();

        Initialize();
    }

    private void Initialize()
    {
        const int START_IDX = 5;
        const int LAST_IDX = 36;
        const string END_LINE = "\r\n";

        string[] data;
        using (var streamReader = new StreamReader(_filename))
            data = streamReader.ReadToEnd().Split(END_LINE);

        foreach (var entry in data)
        {
            var firstEntry = entry[START_IDX];
            var lastEntry = entry[LAST_IDX];

            Node lastNode;
            if (_nodeMap.TryGetValue(lastEntry, out Node? lastValue))
                lastNode = lastValue;
            else
            {
                lastNode = new Node(lastEntry);
                _nodeMap.Add(lastEntry, lastNode);
            }

            if (_nodeMap.TryGetValue(firstEntry, out Node? firstValue))
                lastNode.AddRequirement(firstValue);
            else
            {
                var firstNode = new Node(firstEntry);
                lastNode.AddRequirement(firstNode);
                _nodeMap.Add(firstNode.Name, firstNode);
            }
        }
    }

    public void SolvePart1()
    {
        Console.Write("Part 1. ");

        while (_nodeMap.Count > 0)
        {
            var nodeToLaunch = _nodeMap.Values
                .Where(x => !x.Launched && x.MeetsRequirements)
                .OrderBy(x => x.Name)
                .First();

            nodeToLaunch.Launch();
            _nodeMap.Remove(nodeToLaunch.Name);
        }

        Console.WriteLine();
    }

    public void SolvePart2()
    {
        //const int WORKERS_COUNT = 2;
        const int WORKERS_COUNT = 5;
        var workers = InitializeWorkers(WORKERS_COUNT);

        Console.Write("Part 2. ");

        while (_nodeMap.Count > 0)
        {
            var busyWorkers = workers.Where(x => x.IsBusy).ToList();
            if (busyWorkers.Count != 0)
            {
                var secondsToSkip = busyWorkers.Min(x => x.TimeNeeded - x.BusyTime);
                foreach (var worker in workers)
                    if (worker.IsBusy)
                        worker.AddBusyTime(secondsToSkip);
                    else
                        worker.AddUpTime(secondsToSkip);
            }

            var readyToLaunch = busyWorkers.Where(x => x.IsReadyToLaunch).OrderBy(x => x.Node.Name).ToList();
            foreach (var worker in readyToLaunch)
            {
                worker.Node?.Launch();
                _nodeMap.Remove(worker.Node.Name);
                worker.ReleaseNode();
            }

            var nodesToProcess = _nodeMap.Values
                .Where(x => !x.ReservedByWorker && !x.Launched && x.MeetsRequirements)
                .OrderBy(x => x.Name)
                .ToList();

            var nodesToProcessCount = nodesToProcess.Count;
            var setupedNodesIdx = 0;
            var freeWorkers = workers.Where(x => !x.IsBusy).ToList();
            foreach (var freeWorker in freeWorkers)
            {
                if (setupedNodesIdx >= nodesToProcessCount)
                    break;

                var nodeToProcess = nodesToProcess[setupedNodesIdx];
                freeWorker.SetNode(nodeToProcess);
                ++setupedNodesIdx;
            }
        }

        Console.WriteLine($"\nTotal seconds needed: {workers.Max(x => x.TotalUpTime)}\n");
    }

    private static Worker[] InitializeWorkers(int count)
    {
        var workers = new Worker[count];
        for (int i = 0; i < count; ++i)
            workers[i] = new Worker();

        return workers;
    }
}

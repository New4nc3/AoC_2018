class NodeProcessor
{
    public int[] RawInput { get; }
    public Dictionary<char, Node> Nodes { get; }

    private int _carette;

    public NodeProcessor(string input)
    {
        _carette = 0;
        RawInput = ReadFromFile(input);
        Nodes = new Dictionary<char, Node>();

        InitializeNodes();
    }

    private int[] ReadFromFile(string input)
    {
        using (var streamReader = new StreamReader(input))
            return streamReader.ReadToEnd()
                .Split(" ")
                .Select(x => Convert.ToInt32(x))
                .ToArray();
    }

    private void InitializeNodes()
    {
        var length = RawInput.Length;
        while (_carette != length)
        {
            var childCount = RawInput[_carette++];
            var metadataCount = RawInput[_carette++];

            ProcessNodeRecursively(childCount, metadataCount);
        }
    }

    private Node ProcessNodeRecursively(int childCount, int metadataCount)
    {
        var node = new Node(childCount, metadataCount);
        Nodes.Add(node.Name, node);

        for (var i = 0; i < childCount; ++i)
        {
            var innerChildCound = RawInput[_carette++];
            var innerMetadataCount = RawInput[_carette++];

            node.ChildNodes[i] = ProcessNodeRecursively(innerChildCound, innerMetadataCount);
        }

        for (var i = 0; i < metadataCount; ++i)
            node.Metadatas[i] = RawInput[_carette++];

        return node;
    }

    public int Part1() =>
        Nodes.Values
            .SelectMany(x => x.Metadatas)
            .Sum();
}

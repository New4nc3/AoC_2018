class Node
{
    private static char _nameCounter = 'A';

    public char Name { get; }
    public Node[] ChildNodes { get; private set; }
    public int[] Metadatas { get; private set; }

    public Node(int childCount, int metadataCount)
    {
        Name = _nameCounter++;
        ChildNodes = new Node[childCount];
        Metadatas = new int[metadataCount];
    }

    public override string ToString() =>
        $"{Name} {ChildNodes.Length} {Metadatas.Length}";
}

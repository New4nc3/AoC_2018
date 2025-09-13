class Node
{
    private static char _nameCounter = 'A';
    private int _cachedValue = -1;

    public char Name { get; }
    public Node[] ChildNodes { get; private set; }
    public int[] Metadatas { get; private set; }

    public int Value
    {
        get
        {
            if (_cachedValue != -1)
                return _cachedValue;

            var childCount = ChildNodes.Length;
            if (childCount == 0)
            {
                _cachedValue = Metadatas.Sum();
            }
            else
            {
                var metadataCount = Metadatas.Length;
                _cachedValue = 0;

                for (var i = 0; i < metadataCount; ++i)
                {
                    var index = Metadatas[i] - 1;
                    if (index < 0 || index >= childCount)
                        continue;

                    _cachedValue += ChildNodes[index].Value;
                }
            }

            return _cachedValue;
        }
    }

    public Node(int childCount, int metadataCount)
    {
        Name = _nameCounter++;
        ChildNodes = new Node[childCount];
        Metadatas = new int[metadataCount];
    }

    public override string ToString() =>
        $"{Name} {ChildNodes.Length} {Metadatas.Length} {Value}";
}

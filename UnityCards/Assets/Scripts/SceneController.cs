using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SceneController : MonoBehaviour
{
    public const int gridRows = 3;
    public const int gridCols = 4;
    public const float offsetX = 4f;
    public const float offsetY = 5f;
    public const float originalX = -5;
    public const float originalY = -5;
    public MemoryCard originalCard;
    public Sprite[] images;
    private MemoryCard _firstRevealed;
    private MemoryCard _secondRevealed;
    // Start is called before the first frame update
    void Start()
    {
        int[] numbers = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };
        numbers = ShuffleArray(numbers);
    
        for (int i = 0; i < gridCols; i++)
        {
            for (int j = 0; j < gridRows; j++)
            {
                MemoryCard card = Instantiate(originalCard) as MemoryCard;
                int index = j * gridCols + i;
                int id = numbers[index];
                float posX = (offsetX * i) + originalX;
                float posY = (offsetY * j) + originalY;
                card.transform.position = new Vector3(posX, posY, 2);

                card.SetCard(id, images[id]);
    
            }
        }
    }
    private int[] ShuffleArray(int[] numbers) {
        int[] newArray = numbers.Clone() as int[];
        for (int i=0; i < newArray.Length; i++) {
            int tmp = newArray[i];
            int r = Random.Range(i, newArray.Length);
            newArray[i] = newArray[r];
            newArray[r] = tmp;
        }
        return newArray;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
    public bool canReveal
    {
        get { return _secondRevealed == null;}
    }
    public void CardRevealed(MemoryCard card)
    {
        if (_firstRevealed == null)
        {
            _firstRevealed = card;
        }
        else
        {
            _secondRevealed = card;
            StartCoroutine(CheckMatch());
        }
    }
    private IEnumerator CheckMatch()
    {
        if (_firstRevealed.id == _secondRevealed.id)
        {
            //
        }
        else
        {
            yield return new WaitForSeconds(1.5f);
            _firstRevealed.Unreveal();
            _secondRevealed.Unreveal();
        }
        _firstRevealed = null;
        _secondRevealed = null;
    }
}

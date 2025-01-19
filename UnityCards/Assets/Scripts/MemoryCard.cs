using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MemoryCard : MonoBehaviour
{
    public GameObject cardBackPrefab; // Assign the prefab in the Inspector
    private GameObject cardBackInstance;
    private int _id;

    public int id
    {
        get { return _id; }
    }

    void Start()
    {
    }

    void Update()
    {
    }

    public void SetCard(int id, Sprite image)
    {
        _id = id;
        GetComponent<SpriteRenderer>().sprite = image;

        // Instantiate a new card back instance if not already created
        if (cardBackInstance == null)
        {
            cardBackInstance = Instantiate(cardBackPrefab, transform.position, Quaternion.identity);
            cardBackInstance.transform.SetParent(transform, false); // Parent to the MemoryCard
            cardBackInstance.GetComponent<SpriteRenderer>().sortingOrder = 1; // Ensure it is behind the front card
            cardBackInstance.transform.localPosition = Vector3.zero; // Align to MemoryCard
        }

        cardBackInstance.SetActive(true); // Make sure it's visible
    }

    private void OnMouseDown()
    {
        if (cardBackInstance.activeSelf && FindObjectOfType<SceneController>().canReveal == true)
        {
            cardBackInstance.SetActive(false); // Hide the card back when clicked
            FindObjectOfType<SceneController>().CardRevealed(this);
        }
    }

    public void Unreveal()
    {
        if (cardBackInstance != null)
        {
            cardBackInstance.SetActive(true); // Show the card back again
        }
    }
}


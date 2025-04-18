using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System;
public class Arduino : MonoBehaviour
{
    // Start is called before the first frame update
    public float RotX, RotY, RotZ;
    public bool forward, backward, left, right;
    public float deadZone = 0.5f;
    public Rigidbody rb;
    public float force;
    public float angular;
    public GameObject N, F, B, L, R;

    public string[] datas;
    void Start()
    {
    }

    // Update is called once per frame
    void Update()
    {
        //StartCoroutine(Direction());
        UI();

        if (forward){
            rb.AddForce(rb.transform.forward * force);
        }
        else if (backward){
            rb.AddForce(rb.transform.forward * -force);
        }
        else if (right){
            rb.transform.Rotate(new Vector3(0,angular*Time.deltaTime,0));
        }
        else if (left){
            rb.transform.Rotate(new Vector3(0,-angular*Time.deltaTime,0));
        }
    }

    IEnumerator Direction(){
        
        if(!forward && !backward && !left && !right){
            if (RotZ >= deadZone){
                forward = true;
            }
            else if (RotZ <= -deadZone){
                backward = true;
            }
            else if (RotX >= deadZone){
                left = true;
                L.SetActive(true);
            }
            else if (RotX <= -deadZone){
                right = true;
            }
        }

        else if (forward){
            if (RotZ <= -deadZone){
                forward = false;
            }

        }
        else if (backward){
            if (RotZ >= deadZone){
                backward = false;
            }
        }
        else if (right){
            if (RotX >= deadZone){
                right = false;
            }
        }
        else if (left){
            if (RotX <= -deadZone){
                left = false;
            }
        }
        yield return new WaitForSeconds(1);
    }

    private void UI(){
        N.SetActive(false);F.SetActive(false);B.SetActive(false);L.SetActive(false);R.SetActive(false);
        if(forward){F.SetActive(true);}
        else if(backward){B.SetActive(true);}
        else if(left){L.SetActive(true);}
        else if(right){R.SetActive(true);}
        else{N.SetActive(true);}


    }
     void OnMessageArrived(string msg)
    {
        Debug.Log("Message arrived: " + msg);
        string[] datas = msg.Split(",");
        RotX = float.Parse(datas[0]);
        RotY = float.Parse(datas[1]);
        RotZ = float.Parse(datas[2]);
        StartCoroutine(Direction());
    }
    void OnConnectionEvent(bool success)
    {
        if (success)
            Debug.Log("Connection established");
        else
            Debug.Log("Connection attempt failed or disconnection detected");
    }
}

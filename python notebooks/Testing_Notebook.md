# Quick Guide For Testing
This file will cover how to get your `Testing_Notebook.md` up and working.

## Getting Started

1) The code beginning with
    ```ipynb
    !wget --load-cookies /tmp/cookies.txt ....
    ```
    is one of the important code lines in the code because it is responsible for downloading the weights file from the Google Drive.

    **Weights** is what you get after training your model.

    In the same code above you may have noticed that there is a Google document link present in the code. This code has a ID present in the Google Document link.

    To get the ID all you have to is:
    - Upload the weights file to your Google Drive. 
    - *Share* the file (to everyone) and copy the generated link.
    - In the link the alphanumeric code present between */d/* and */view/* is your ID.

    Replace the ID with the one in the code above.

    At the end of this code line before `rm -rf /tmp/cookies.txt` replace the `yolov4_custom_train_1_last.weights` with the your file name.

2) To get a feel of the working of the MQTT server with the detection, under "Running on Webcam Video" inside `client.connect` change `your.mqtt.server.com` with your intended MQTT server address.
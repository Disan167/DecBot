import vosk
import pyaudio
import json


model = vosk.Model("C:/Users/C/DecBot/vosk-model-small-en-us-0.15")  
recognizer = vosk.KaldiRecognizer(model, 16000)


p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paInt16, channels=1, rate=16000, input=True, frames_per_buffer=8000)
stream.start_stream()

print("Listening... Say 'stop listening' to stop.")

while True:
    data = stream.read(4000)
    if recognizer.AcceptWaveform(data):
        result = recognizer.Result()
        result_json = json.loads(result)
        recognized_text = result_json['text']
        print(f"Recognized: {recognized_text}")
        
        
        if "stop listening" in recognized_text:
            print("Stopping listening...")
            break
    else:
        partial_result = recognizer.PartialResult()
        partial_result_json = json.loads(partial_result)
        print(f"Partial: {partial_result_json['partial']}")


stream.stop_stream()
stream.close()
p.terminate()
print("Program terminated.")

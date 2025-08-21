import asyncio
from bleak import BleakClient
import time
import os
import numpy as np
from scipy.signal import find_peaks

ADDRESS = "68:5E:1C:33:88:56"
CHAR_UUID = "0000ffe1-0000-1000-8000-00805f9b34fb"
DELAY = 0.4

async def play_song(sd, sf, wav_file_name, client, beat_times):
    with sf.SoundFile(wav_file_name) as f:
        samplerate = f.samplerate
        data = f.read()
    
    sd.play(data, samplerate)
    start_time = time.perf_counter()
    for ts in beat_times:
        ts = ts + DELAY
        now = time.perf_counter()
        sleep_time = ts - (now - start_time)
        if sleep_time > 0:
            await asyncio.sleep(sleep_time)
        color = b"ffffff"
        await client.write_gatt_char(CHAR_UUID, color)

def get_bass_peak_timestamps(y, sr, low_freq_limit=1000, height_ratio=0.5):
    import librosa

    S = np.abs(librosa.stft(y, n_fft=2048, hop_length=512))
    freqs = librosa.fft_frequencies(sr=sr)

    low_bins = freqs <= low_freq_limit
    bass_energy = S[low_bins, :].mean(axis=0)

    peaks, _ = find_peaks(bass_energy, height=np.max(bass_energy) * height_ratio, distance=1)
    peak_times = librosa.frames_to_time(peaks, sr=sr, hop_length=512)
    return peak_times.tolist()

async def main():
    async with BleakClient(ADDRESS) as client:
        if client.is_connected:
            print("Connected to HT-08")

            import soundfile as sf
            import sounddevice as sd
            import librosa

            print("Available songs:")
            for song_title in os.listdir(r"songs"):
                print(song_title[:-4])
            song = input("Type the song to play: ")
            while not os.path.isfile(rf"songs\{song}.wav"):
                song = input("Invalid song. Type the song to play. ")
            y, sr = librosa.load(rf"songs\{song}.wav")

            #TEMPO/BEAT
            #tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)
            #timestamps = librosa.frames_to_time(beat_frames, sr=sr)

            #VOICE
            #onset_frames = librosa.onset.onset_detect(y=y, sr=sr)
            #timestamps = librosa.frames_to_time(onset_frames, sr=sr)

            #BASS
            timestamps = get_bass_peak_timestamps(y, sr)
            await play_song(sd, sf, rf"songs\{song}.wav", client, timestamps)
asyncio.run(main())
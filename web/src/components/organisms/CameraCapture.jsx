import { useEffect, useRef, useState } from "react";
import { createWorker } from "tesseract.js";

export default function CameraCapture() {
  const videoRef = useRef(null);
  const canvasRef = useRef(null);

  const [photo, setPhoto] = useState(null);
  const [delay, setDelay] = useState(2000);

  async function startCamera() {
    try {
      const stream = await navigator.mediaDevices.getUserMedia({
        video: { facingMode: "environment" },
      });
      videoRef.current.srcObject = stream;
    } catch (err) {
      console.error("Erro ao abrir câmera:", err);
    }
  }

  function stopCamera() {
    const stream = videoRef.current?.srcObject;
    if (!stream) return;
    stream.getTracks().forEach((track) => track.stop());
  }

  async function takePicture() {
    return new Promise((resolve) => {
      setTimeout(() => {
        const video = videoRef.current;
        const canvas = canvasRef.current;

        canvas.width = video.videoWidth;
        canvas.height = video.videoHeight;

        const ctx = canvas.getContext("2d");
        ctx.drawImage(video, 0, 0, canvas.width, canvas.height);

        const image = canvas.toDataURL("image/jpeg");
        setPhoto(image);
        resolve(image);
      }, delay);
    });
  }

  async function tesseract(image) {
    const worker = await createWorker("por");
    const ret = await worker.recognize(image);
    console.log(ret.data.text);
    await worker.terminate();
  }

  useEffect(() => {
    startCamera();
    return () => stopCamera();
  }, []);

  return (
    <div className="grid grid-cols-4 gap-4 col-span-4">
      <button
        onClick={async () => {
          const image = await takePicture();
          await tesseract(image);
        }}
        className="col-span-1 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg" // ✅ Fix 3
      >
        Tirar Foto
      </button>

      <div className="h-40 col-span-1 bg-zinc-800 rounded-xl border border-zinc-700 p-4 flex flex-col justify-center">
        <label className="text-sm text-zinc-400 mb-2">Picture Delay (ms)</label>
        <input
          type="number"
          value={delay}
          onChange={(e) => setDelay(Number(e.target.value))}
          className="bg-zinc-700 rounded-lg p-2 outline-none"
        />
      </div>

      <video
        ref={videoRef}
        autoPlay
        playsInline
        className="col-span-1 rounded-xl border border-zinc-700"
      />

      <canvas ref={canvasRef} className="hidden" />

      {photo && (
        <img
          src={photo}
          alt="foto"
          className="col-span-1 rounded-xl border-2 border-red-500"
        />
      )}
    </div>
  );
}
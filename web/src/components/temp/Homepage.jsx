import ItemHolder from "../atoms/holders/ItemHolder";
import CentralTitle from "../atoms/titles/CentralTitle";

import LocalStorage from "../../utils/localStorageSaver";
import CameraCapture from "../organisms/CameraCapture";
import { useEffect, useState } from "react";
import { log } from "@techstark/opencv-js";

export default function Homepage() {
  const [data, setData] = useState({});

  const date = new Date();
  const handleEventInput = (e) => {
    LocalStorage.handleEventInput(e, data, setData);
  };

  const sendActionTimeStamp = (key) => {
    let timeNow = performance.now();
    LocalStorage.insertFakeValue(key, timeNow, data, setData);
  };

  useEffect(() => {
    //WIP temporario
    console.log(date.getTime()); // Obtem tempo em milisegundos desde que a página foi aberta, pior opção no caso.
    console.log(performance.now() * 1000); // Obtem microsegundos desde que a página foi aberta, boa para melhor precisão!
    console.log(data); // Dados armazenados nos inputs
  }, [data]);

  function handleSubmit(e) {
    e.preventDefault(); // impede o reload

    console.log("Enviado");
  }

  // No momento em que se tira a foto é necessário pegar um timeStamp
  // No momento em que se envia também é necessário pegar outro timeStamp

  return (
    <ItemHolder
      elements={
        <>
          <CentralTitle text={"Camera Timing Config"} />

          <form onSubmit={handleSubmit}>
            <div className="grid grid-cols-4 gap-4">
              {/* Hit Score */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo Alvo
              </div>
              <div className="col-span-3">
                <input
                  type="number"
                  name="hitScore"
                  placeholder="10000"
                  onChange={handleEventInput}
                  className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-500/30"
                />
              </div>

              {/* Delay */}
              <CameraCapture
                insertFakeValue={sendActionTimeStamp}
                onPhotoTaken={() => sendActionTimeStamp("pictureTime")}
              />

              {/* Set Unity */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo na foto
              </div>

              <div className="col-span-3">
                <input
                  type="number"
                  name="pictureTimeFrame"
                  placeholder="10000"
                  onChange={handleEventInput}
                  className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-500/30"
                />
              </div>

              {/* Measurement Unity */}
              <div className="flex items-center font-medium text-zinc-300">
                Unidade de medida
              </div>

              <div className="col-span-3">
                <select
                  className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500"
                  onChange={handleEventInput}
                  name="measurementUnity"
                >
                  <option value={"microsecond"}>
                    Microsegundos 0,000001, 1seg = 1.000.000us
                  </option>
                  <option value={"milisecond"}>
                    Milliseconds 0,001, 1seg = 1,000ms{" "}
                  </option>
                  <option value={"second"}>Seconds 1</option>
                </select>
              </div>

              {/* timeMissCorrection */}
              <div className="flex items-center font-medium text-zinc-300">
                Time Scored
              </div>

              <div className="col-span-3">
                <input
                  type="number"
                  name="timeScored"
                  placeholder="0"
                  onChange={handleEventInput}
                  className="w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500"
                />
              </div>

              {/* Send Address */}
              <div className="flex items-center font-medium text-zinc-300">
                Adress
              </div>

              <div className="col-span-3">
                <input
                  type="text"
                  name="postAddress"
                  placeholder="192.168.0.1/source?"
                  onChange={handleEventInput}
                  className="w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500"
                />
              </div>

              <button
                onClick={() => sendActionTimeStamp("sendTime")}
                className="col-span-4 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg"
              >
                Enviar
              </button>

              <div className="col-span-4 border-2 border-amber-200">
                <h1 className="text-center col-span-4 text-2xl">LOG</h1>
              </div>
              <div className="col-span-4"></div>
            </div>
          </form>
        </>
      }
    />
  );
}

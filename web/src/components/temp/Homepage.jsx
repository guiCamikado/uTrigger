import ItemHolder from "../atoms/holders/ItemHolder";
import CentralTitle from "../atoms/titles/CentralTitle";

import LocalStorage from "../../utils/localStorageSaver";
import CameraCapture from "../organisms/CameraCapture";
import { useEffect } from "react";

export default function Homepage() {
  useEffect(() => {
    LocalStorage.setData("DataToESP32", { chave: "valor", key: "value" });
  }, []);

  return (
    <ItemHolder
      elements={
        <>
          <CentralTitle text={"Camera Timing Config"} />

          <div className="grid grid-cols-4 gap-4">
            {/* Hit Score */}
            <div className="flex items-center font-medium text-zinc-300">
              Set HitScore Time
            </div>

            <div className="col-span-3">
              <input
                type="number"
                placeholder="10000"
                className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-500/30"
              />
            </div>

            {/* Delay */}
            <CameraCapture />

            {/* Unidade */}
            <div className="flex items-center font-medium text-zinc-300">
              Time Unit
            </div>

            <div className="col-span-3">
              <select className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500">
                <option>Microseconds</option>
                <option>Milliseconds</option>
                <option>Seconds</option>
              </select>
            </div>

            {/* Tempo de correção */}
            <div className="flex items-center font-medium text-zinc-300">
              Error Correction
            </div>

            <div className="col-span-3">
              <input
                type="number"
                placeholder="0"
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
                placeholder="192.168.0.1/source?"
                className="w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500"
              />
            </div>
          </div>
        </>
      }
    />
  );
}

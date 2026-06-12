import { useEffect, useState } from "react";
import ItemHolder from "../atoms/ItemHolder";
import CentralTitle from "../atoms/CentralTitle";
import DefaultInput from "../atoms/DefaultInput";
import LocalStorage from "../../utils/LocalStorageSaver";
import HttpClient from "../../utils/httpClient";
import DefaultButton from "../atoms/DefaultButtons";

export default function Homepage() {
  const [data, setData] = useState({});
  const [urlLink, setUrlLink] = useState("");
  const [ipToSend, setIpToSend] = useState("192.168.1.1/control");

  const handleEventInput = (e) => {
    LocalStorage.handleEventInput(e, data, setData);
  };

  const sendData = (urlLink, data) => {
    HttpClient.sendData(urlLink, data);
  };

  function handleSubmit(e) {
    e.preventDefault();
    console.log("Enviado");
  }

  return (
    <ItemHolder
      elements={
        <>
          <CentralTitle text={"Configurar"} />
          <form onSubmit={handleSubmit}>
            <div className="grid grid-cols-4 gap-4">
              {/* Hit Score */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo Alvo:
              </div>
              <div className="col-span-3">
                <DefaultInput
                  type={"number"}
                  name={"hitscore"}
                  placeholder={"10000"}
                  onChange={handleEventInput}
                />
              </div>

              {/* timeMissCorrection */}
              <div className="flex items-center font-medium text-zinc-300">
                Tempo após tentativa:
              </div>

              <div className="col-span-3">
                <DefaultInput
                  type={"number"}
                  name={"timeScored"}
                  placeholder={"0"}
                  onChange={handleEventInput}
                />
              </div>

              {/* Send Address */}
              <div className="flex items-center font-medium text-zinc-300">
                Endereço de Envio:
              </div>

              <div className="col-span-3">
                <DefaultInput
                  type="text"
                  name="postAddress"
                  placeholder="192.168.0.1/source?"
                  value={ipToSend}
                  onChange={(e) => {
                    setIpToSend(e.target.value);
                    handleEventInput(e);
                    setUrlLink("http://" + e.target.value);
                  }}
                />
              </div>

              <DefaultButton
                onClick={() => {
                  const timeNow = performance.now() * 1000;
                  const finalData = {
                    ...data,
                    sendTime: timeNow,
                    reset: false,
                  };
                  LocalStorage.insertFakeValue(
                    "sendTime",
                    timeNow,
                    finalData,
                    setData,
                  );
                  sendData(urlLink, finalData);
                }}
                className={
                  "col-span-3 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg"
                }
                Text={"Iniciar Dispositivo"}
              />

              <DefaultButton
                onClick={() => {
                  const finalData = {
                    ...data,
                    reset: true,
                  };
                  LocalStorage.insertFakeValue(
                    "sendTime",
                    { reset: true },
                    finalData,
                    setData,
                  );
                  sendData(urlLink, finalData);
                }}
                className={
                  "col-span-1 h-40 rounded-xl bg-yellow-600 hover:bg-yellow-500 transition text-xl font-bold shadow-lg"
                }
                Text={"Reiniciar Dispositivo"}
              />
            </div>
          </form>
        </>
      }
    />
  );
}

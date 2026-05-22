export default function Homepage() {
  return (
    <div className="min-h-screen bg-zinc-950 text-zinc-100 p-6">
      <div className="max-w-4xl mx-auto bg-zinc-900 border border-zinc-800 rounded-2xl p-6 shadow-2xl">
        <h1 className="text-2xl font-bold mb-6 text-center">
          Camera Timing Config
        </h1>

        <div className="grid grid-cols-3 gap-4">
          {/* Hit Score */}
          <div className="flex items-center font-medium text-zinc-300">
            Set HitScore Time
          </div>

          <div className="col-span-2">
            <input
              type="number"
              placeholder="10000"
              className=" w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500 focus:ring-2 focus:ring-blue-500/30"
            />
          </div>

          {/* Botão foto */}
          <button className=" col-span-2 h-40 rounded-xl bg-blue-600 hover:bg-blue-500 transition text-xl font-bold shadow-lg">
            Tirar Foto
          </button>

          {/* Delay */}
          <div className=" h-40 bg-zinc-800 rounded-xl border border-zinc-700 p-4 flex flex-col justify-center">
            <label className="text-sm text-zinc-400 mb-2">
              Picture Delay (ms)
            </label>

            <input
              type="number"
              defaultValue="2000"
              className=" bg-zinc-700 rounded-lg p-2 outline-none focus:border-blue-500"
            />
          </div>

          {/* Unidade */}
          <div className="flex items-center font-medium text-zinc-300">
            Time Unit
          </div>

          <div className="col-span-2">
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

          <div className="col-span-2">
            <input
              type="number"
              placeholder="0"
              className="w-full bg-zinc-800 border border-zinc-700 rounded-lg p-3 outline-none focus:border-blue-500"
            />
          </div>
        </div>
      </div>
    </div>
  );
}

#pragma once

const char INDEX_HTML[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Control</title>
    <style>
        body { font-family: sans-serif; display: flex; flex-direction: column;
               align-items: center; justify-content: center; height: 100vh; margin: 0; }
        button { padding: 20px 40px; font-size: 1.2rem; margin: 10px;
                 border: none; border-radius: 8px; cursor: pointer; }
        #btn-on  { background: #4CAF50; color: white; }
        #btn-off { background: #f44336; color: white; }
        #status  { margin-top: 20px; font-size: 1rem; color: #555; }
    </style>
</head>
<body>
    <h2>Controle ESP32</h2>
    <button id="btn-on"  onclick="send(1)">LIGAR</button>
    <button id="btn-off" onclick="send(0)">DESLIGAR</button>
    <p id="status">Aguardando...</p>

    <script>
        function send(val) {
            fetch('/control', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ value: val })
            })
            .then(r => r.json())
            .then(data => {
                document.getElementById('status').innerText =
                    'Enviado: ' + (data.value === 1 ? 'LIGADO' : 'DESLIGADO');
            })
            .catch(() => {
                document.getElementById('status').innerText = 'Erro ao enviar';
            });
        }
    </script>
</body>
</html>
)rawhtml";
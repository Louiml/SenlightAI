/* ============ Senlight Affective AI — frontend logic ============ */
const $ = (id) => document.getElementById(id);

const els = {
  messages: $("messages"),
  input: $("input"),
  composer: $("composer"),
  btnSend: $("btnSend"),
  btnClear: $("btnClear"),
  pMax: $("pMax"), pTemp: $("pTemp"), pTopK: $("pTopK"),
  emotion: $("emotionLabel"), vadText: $("vadText"), conf: $("confText"),
  barV: $("barV"), barA: $("barA"), barD: $("barD"),
  aurora: $("aurora"),
};

// emotion -> color used for the badge + page aura
const EMO = {
  "Joy": "#22c55e", "Trust": "#14b8a6", "Anticipation": "#f59e0b",
  "Surprise": "#f59e0b", "Sadness": "#60a5fa", "Fear": "#a855f7",
  "Disgust": "#64748b", "Anger": "#f87171", "Neutral": "#64748b",
};

let context = "";   // running conversation context for the engine
let busy = false;

/* ---------- helpers ---------- */
function esc(s) {
  const d = document.createElement("div");
  d.textContent = s;
  return d.innerHTML;
}

function addMsg(kind, html) {
  const div = document.createElement("div");
  div.className = "msg " + kind;
  div.innerHTML = html;
  els.messages.appendChild(div);
  els.messages.scrollTop = els.messages.scrollHeight;
  return div;
}

function setEmotion(emotion, vad, conf) {
  els.emotion.textContent = emotion;
  const c = EMO[emotion] || "#64748b";
  els.emotion.style.background = c + "33";
  els.emotion.style.color = c;
  els.emotion.style.boxShadow = `0 0 24px ${c}55`;
  els.vadText.textContent = `V ${vad[0].toFixed(2)} · A ${vad[1].toFixed(2)} · D ${vad[2].toFixed(2)}`;
  els.conf.textContent = conf ? `signal ${Math.round(conf * 100)}%` : "signal —";
  els.barV.style.width = (50 + 50 * vad[0]) + "%";
  els.barA.style.width = (50 + 50 * vad[1]) + "%";
  els.barD.style.width = (50 + 50 * vad[2]) + "%";
  els.aurora.style.background =
    `radial-gradient(600px 400px at 20% 80%, ${hexA(c, 0.16)}, transparent 60%),` +
    `radial-gradient(500px 380px at 85% 20%, ${hexA(c, 0.12)}, transparent 60%)`;
}

function hexA(hex, a) {
  const r = parseInt(hex.slice(1, 3), 16), g = parseInt(hex.slice(3, 5), 16),
        b = parseInt(hex.slice(5, 7), 16);
  return `rgba(${r},${g},${b},${a})`;
}

/* ---------- bridge: desktop (pywebview) vs web (fetch) ---------- */
function hasBridge() {
  return typeof window.pywebview !== "undefined" && window.pywebview.api;
}

async function apiChat(payload) {
  if (hasBridge()) {
    return window.pywebview.api.chat(
      payload.prompt,
      payload.max_tokens,
      payload.temperature,
      payload.top_k,
      payload.context,
    );
  }
  const r = await fetch("/api/chat", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });
  return r.json();
}

async function apiDetect(text) {
  if (hasBridge()) {
    return window.pywebview.api.detect_emotion(text);
  }
  const r = await fetch("/api/emotion?text=" + encodeURIComponent(text));
  return r.json();
}

/* ---------- live emotion detection ---------- */
async function detectEmotion(text) {
  try {
    const d = await apiDetect(text);
    setEmotion(d.emotion || "Neutral", d.vad || [0, 0, 0], d.confidence);
  } catch (_) { /* ignore network glitches */ }
}

els.input.addEventListener("input", () => {
  detectEmotion(els.input.value);
  els.input.style.height = "auto";
  els.input.style.height = Math.min(els.input.scrollHeight, 140) + "px";
});

/* ---------- chat ---------- */
els.composer.addEventListener("submit", (e) => {
  e.preventDefault();
  const text = els.input.value.trim();
  if (!text || busy) return;
  els.input.value = "";
  els.input.style.height = "auto";
  send(text);
});

async function send(text) {
  busy = true;
  els.btnSend.disabled = true;
  addMsg("user", esc(text));

  const cap = addMsg("cap", "analyzing emotion…");
  const t = addMsg("ai", '<span class="typing"><i></i><i></i><i></i></span>');

  const body = {
    prompt: text,
    max_tokens: parseInt(els.pMax.value) || 90,
    temperature: parseFloat(els.pTemp.value) || 0.75,
    top_k: parseInt(els.pTopK.value) || 40,
    context: context,
  };

  try {
    const d = await apiChat(body);

    cap.textContent =
      `detected ${d.plutchik} · V ${(d.vad[0]).toFixed(2)} ` +
      `A ${(d.vad[1]).toFixed(2)} D ${(d.vad[2]).toFixed(2)}`;
    setEmotion(d.emotion || "Neutral", d.vad || [0, 0, 0], d.confidence);

    t.className = "msg ai";
    t.textContent = d.reply || "(empty reply)";
    context = d.context || "";
  } catch (err) {
    cap.textContent = "engine error";
    t.className = "msg err";
    t.textContent = "Could not reach the engine: " + err;
  } finally {
    busy = false;
    els.btnSend.disabled = false;
    els.input.focus();
    els.messages.scrollTop = els.messages.scrollHeight;
  }
}

/* ---------- new session ---------- */
els.btnClear.addEventListener("click", () => {
  context = "";
  els.messages.innerHTML =
    '<div class="msg system"><div>New session started. How are you feeling?</div></div>';
  setEmotion("Neutral", [0, 0, 0], 0);
  els.input.value = "";
  els.input.focus();
});

/* ---------- init ---------- */
// auto-detect while typing placeholder default
setEmotion("Neutral", [0, 0, 0], 0);
els.input.focus();
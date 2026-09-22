/* ────────────────────────────────────────────────────────────────
   Aurora Glass — app logic (pywebview bridge + fetch fallback)
   ──────────────────────────────────────────────────────────────── */
const $ = (id) => document.getElementById(id);

const els = {
  messages: $("messages"),
  input: $("input"),
  composer: $("composer"),
  btnSend: $("btnSend"),
  btnNew: $("btnNew"),
  pMax: $("pMax"), pTemp: $("pTemp"), pTopK: $("pTopK"),
  vadEmotion: $("vadEmotion"),
  vadCoords: $("vadCoords"),
  vadConf: $("vadConf"),
  barV: $("barV"), barA: $("barA"), barD: $("barD"),
  plutchikGrid: $("plutchikGrid"),
};

// Plutchik emotions + accent colors (theme-driven orange ramp for positives,
// cooler tones for negatives to keep saliency).
const PLUTCHIK = [
  ["Joy", "#ffc25e"], ["Trust", "#ffa940"], ["Anticipation", "#ffd9a6"],
  ["Surprise", "#ffb14e"], ["Anger", "#ff7a17"], ["Disgust", "#b9a48c"],
  ["Fear", "#9a8cff"], ["Sadness", "#7fa0ff"],
];

let context = "";
let busy = false;

/* ---------- bridge ---------- */
function hasBridge() {
  return typeof window.pywebview !== "undefined" && !!window.pywebview.api;
}

async function apiChat(payload) {
  if (hasBridge()) {
    return window.pywebview.api.chat(
      payload.prompt, payload.max_tokens, payload.temperature, payload.top_k, payload.context
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
  if (hasBridge()) return window.pywebview.api.detect_emotion(text);
  const r = await fetch("/api/emotion?text=" + encodeURIComponent(text));
  return r.json();
}

/* ---------- helpers ---------- */
function addMsg(kind, html) {
  const div = document.createElement("div");
  div.className = "msg " + kind;
  div.innerHTML = html;
  els.messages.appendChild(div);
  els.messages.scrollTop = els.messages.scrollHeight;
  return div;
}

function renderPlutchik() {
  els.plutchikGrid.innerHTML = "";
  PLUTCHIK.forEach(([name, color]) => {
    const c = document.createElement("div");
    c.className = "plutchik-chip";
    c.innerHTML = `<span class="dot" style="background:${color}"></span>${name}`;
    els.plutchikGrid.appendChild(c);
  });
}

function setEmotion(emotion, vad, conf) {
  els.vadEmotion.textContent = emotion;
  els.vadCoords.textContent =
    `V ${vad[0].toFixed(2)} · A ${vad[1].toFixed(2)} · D ${vad[2].toFixed(2)}`;
  els.vadConf.textContent = conf ? `signal ${Math.round(conf * 100)}%` : "signal —";
  els.barV.style.width = (50 + 50 * vad[0]) + "%";
  els.barA.style.width = (50 + 50 * vad[1]) + "%";
  els.barD.style.width = (50 + 50 * vad[2]) + "%";
  [els.barV, els.barA, els.barD].forEach(b => b.classList.add("is-armed"));
}

async function detectEmotion(text) {
  try {
    const d = await apiDetect(text);
    setEmotion(d.emotion || "Neutral", d.vad || [0, 0, 0], d.confidence);
  } catch (_) { /* ignore */ }
}

/* ---------- input (auto-grow + live detect) ---------- */
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
  addMsg("user", text.replace(/</g, "&lt;"));
  const cap = addMsg("cap", "analyzing emotion…");
  const t = addMsg("ai", '<span class="typing"><i></i><i></i><i></i></span>');

  const body = {
    prompt: text,
    max_tokens: parseInt(els.pMax.value) || 90,
    temperature: parseFloat(els.pTemp.value) || 0.75,
    top_k: parseInt(els.pTopK.value) || 40,
    context,
  };

  try {
    const d = await apiChat(body);
    cap.innerHTML = `detected ${d.plutchik} · V ${(d.vad[0]).toFixed(2)} ` +
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
els.btnNew.addEventListener("click", () => {
  context = "";
  els.messages.innerHTML =
    '<div class="msg system"><div>New session started. How are you feeling?</div></div>';
  setEmotion("Neutral", [0, 0, 0], 0);
  els.input.value = "";
  els.input.focus();
});

/* ---------- init ---------- */
renderPlutchik();
setEmotion("Neutral", [0, 0, 0], 0);
els.input.focus();
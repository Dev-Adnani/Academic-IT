let phase = 1; // 1 = full moon, 0 = new moon
let moon = document.getElementById('moon');
let moonSize = 200;

function updateMoon() {
  let phases = ['🌑', '🌒', '🌓', '🌔', '🌕', '🌖', '🌗', '🌘'];
  moon.innerHTML = phases[phase];
  if (phase === 0 || phase === 4) {
    let event = new CustomEvent('moonPhase', { detail: { phase: phase }});
    document.dispatchEvent(event);
  }
}

updateMoon();

document.addEventListener('keydown', function(event) {
  if (event.key === 'ArrowRight') {
    phase = (phase + 1) % 8;
    updateMoon();
  } else if (event.key === 'ArrowLeft') {
    phase = (phase + 7) % 8;
    updateMoon();
  } else if (event.key === 'ArrowUp') {
    moonSize += 10;
    moon.style.fontSize = moonSize + 'px';
  } else if (event.key === 'ArrowDown') {
    moonSize = Math.max(moonSize - 10, 50);
    moon.style.fontSize = moonSize + 'px';
  }
});

moon.addEventListener('wheel', function(event) {
  moonSize += event.deltaY > 0 ? -10 : 10;
  moonSize = Math.max(moonSize, 50);
  moon.style.fontSize = moonSize + 'px';
  event.preventDefault();
});

document.addEventListener('moonPhase', function(event) {
  let phaseName = event.detail.phase === -1 ? 'New Moon' : 'Full Moon';
  alert(phaseName);
});

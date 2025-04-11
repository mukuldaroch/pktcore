const fileInput = document.getElementById("file-input");

fileInput.addEventListener("change", () => {
  const file = fileInput.files[0];
  if (file) {
    alert(`Loaded config: ${file.name}`);
    // here you can send the file path to Electron's main process or whatever logic you need
  }
});

const { execFile } = require("child_process");
const path = require("path");

const cliPath = path.join(__dirname, "..", "build", "fsplit-cli"); // Adjust for your OS

document.getElementById("split").addEventListener("click", () => {
    execFile(
        cliPath,
        ["split", "example.txt", "3"],
        (error, stdout, stderr) => {
            document.getElementById("output").textContent = error
                ? stderr
                : stdout;
        },
    );
});

document.getElementById("combine").addEventListener("click", () => {
    execFile(
        cliPath,
        ["combine", "example.txt.part", "output.txt"],
        (error, stdout, stderr) => {
            document.getElementById("output").textContent = error
                ? stderr
                : stdout;
        },
    );
});

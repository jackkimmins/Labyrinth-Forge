let maze = null;
let playerPosition = { x: 0, y: 0 };
let gridWidth = 0;
let gridHeight = 0;

// Caching DOM Elements
const grid = document.getElementById('grid');
const mazeSeedInput = document.getElementById('mazeSeed');
const mazeSizeInput = document.getElementById('mazeSize');

function GenerateMaze(mazeWidth, mazeHeight, mazeSeed = -1) {
    if (maze === null) {
        console.log("Maze not initialized");
        return;
    }

    maze.Generate(mazeWidth, mazeHeight, mazeSeed);

    const mazeData = maze.MazeToString();
    console.log(mazeData);

    gridWidth = mazeWidth * 2 + 1;
    gridHeight = mazeHeight * 2 + 1;

    const gridStyle = `grid-template-columns: repeat(${gridWidth}, 1fr); grid-template-rows: repeat(${gridHeight}, 1fr);`;

    const htmlMaze = maze.MazeToHTMLString();
    grid.innerHTML = htmlMaze;
    grid.style = gridStyle;

    playerPosition = { x: 1, y: 1 };
}

function GetSizeAndSeed() {
    const urlParams = new URLSearchParams(window.location.search);
    let mazeSeed = urlParams.get('seed') || mazeSeedInput.value || -1;
    let mazeSize = urlParams.get('size') || mazeSizeInput.value || 10;

    if (isNaN(mazeSeed)) mazeSeed = mazeSeed.hashCode();
    return { size: parseInt(mazeSize), seed: parseInt(mazeSeed) };
}

function DownloadMaze() {
    if (maze === null) {
        console.error("No maze to download");
        return;
    }

    const mazeData = maze.MazeToString();
    const blob = new Blob([mazeData], { type: "text/plain" });
    const url = window.URL.createObjectURL(blob);
    const link = document.createElement('a');
    link.href = url;

    const { size: mazeSize, seed: mazeSeed } = GetSizeAndSeed();
    link.download = `maze_${mazeSize}_${mazeSeed}.txt`;
    document.body.appendChild(link);
    link.click();
    document.body.removeChild(link);
}

function MovePlayer(dx, dy) {
    const newPosition = { x: playerPosition.x + dx, y: playerPosition.y + dy };
    const isPositionValid = newPosition.x >= 0 && newPosition.y >= 0 && newPosition.x < gridWidth && newPosition.y < gridHeight;

    if (isPositionValid && !document.querySelector('.cell:nth-child(' + (newPosition.y * gridWidth + newPosition.x + 1) + ')').classList.contains('wall')) {
        document.querySelector('.cell:nth-child(' + (playerPosition.y * gridWidth + playerPosition.x + 1) + ')').classList.remove('player');
        playerPosition = newPosition;
        const newTile = document.querySelector('.cell:nth-child(' + (playerPosition.y * gridWidth + playerPosition.x + 1) + ')');
        newTile.classList.add('player');

        if (newTile.classList.contains('end')) {
            alert('You reached the end of the maze!');
        }
    }
}

Module.onRuntimeInitialized = _ => {
    maze = new Module.Maze();
    GenerateMaze(10, 10, 1);

    document.getElementById('btnGenMaze').addEventListener('click', () => {
        const { size: mazeSize, seed: mazeSeed } = GetSizeAndSeed();
        GenerateMaze(mazeSize, mazeSize, mazeSeed);
    });
};

document.getElementById('btnDownloadMaze').addEventListener('click', DownloadMaze);
document.getElementById('btnGitHubRepo').addEventListener('click', () => window.open('https://github.com/jackkimmins/Labyrinth-Forge', '_blank'));

document.addEventListener('keydown', event => {
    if (event.key === 'Enter') {
        const { size: mazeSize, seed: mazeSeed } = GetSizeAndSeed();
        GenerateMaze(mazeSize, mazeSize, mazeSeed);
    }

    const keyMap = { 'ArrowLeft': [-1, 0], 'ArrowUp': [0, -1], 'ArrowRight': [1, 0], 'ArrowDown': [0, 1] };
    const direction = keyMap[event.key];
    if (direction) MovePlayer(...direction);
});
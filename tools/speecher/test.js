
const { nodewhisper } = require('nodejs-whisper');
const path = require('path');
const fs = require('fs');

const DIR = process.argv[2]
    ? path.resolve(process.argv[2])
    : null;

if (!DIR) {
    console.error('Usage: node transcribe.js <audio-folder>');
    process.exit(1);
}

if (!fs.existsSync(DIR) || !fs.statSync(DIR).isDirectory()) {
    console.error(`Folder does not exist: ${DIR}`);
    process.exit(1);
}

const DB_PATH = path.join(DIR, 'transcriptions.json');

function loadDatabase() {
    if (!fs.existsSync(DB_PATH)) return {};

    return JSON.parse(fs.readFileSync(DB_PATH, 'utf8'));
}

function saveDatabase(db) {
    fs.writeFileSync(DB_PATH, JSON.stringify(db, null, 4) + '\n');
}

async function main() {
    const db = loadDatabase();

    const files = fs.readdirSync(DIR)
        .filter(file => /\.(wav|mp3|m4a|flac|ogg|aac|wma|opus)$/i.test(file))
        .sort((a, b) =>
            a.localeCompare(b, undefined, { numeric: true })
        );

    for (const file of files) {
        const audioPath = path.join(DIR, file);
        const id = path.parse(file).name;

        if (db[id]?.confirmed === true) {
            console.log(`Skipping ${file}: already confirmed.`);
            continue;
        }

        console.log(`Transcribing ${file}...`);

        // Whisper CLI typically writes <audio filename>.txt.
        const transcriptPath = `${audioPath}.txt`;

        try {
            // Remove stale output so it cannot be mistaken for a new transcript.
            if (fs.existsSync(transcriptPath)) {
                fs.unlinkSync(transcriptPath);
            }

            await nodewhisper(audioPath, {
                modelName: 'base.en',
                autoDownloadModelName: 'base.en',
                logger: console,
                whisperOptions: {
                    outputInText: true
                }
            });

            if (!fs.existsSync(transcriptPath)) {
                console.error(`No transcript output found for ${file}`);
                continue;
            }

            const text = fs.readFileSync(transcriptPath, 'utf8').trim();

            db[id] = {
                ...db[id],
                text,
                confirmed: db[id]?.confirmed ?? false
            };

            saveDatabase(db);
            console.log(`Saved transcript for ${file}`);
        } catch (error) {
            console.error(`Failed to transcribe ${file}:`, error.message);
        } finally {
            // Remove Whisper's temporary text output.
            if (fs.existsSync(transcriptPath)) {
                fs.unlinkSync(transcriptPath);
            }
        }
    }

    console.log(`Finished. Results saved to ${DB_PATH}`);
}

main().catch(error => {
    console.error(error);
    process.exitCode = 1;
});

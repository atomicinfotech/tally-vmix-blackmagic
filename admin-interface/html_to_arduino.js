const fs = require('fs');
const path = require('path');
const minify = require('html-minifier').minify;

function minimizeHtml(htmlContent) {
    // First, minimize the main HTML content
    let minimized = minify(htmlContent, {
        removeComments: true,
        collapseWhitespace: true,
        conservativeCollapse: false,
        collapseInlineTagWhitespace: true,
        minifyJS: true,
        minifyCSS: true,
        removeAttributeQuotes: true,
        removeRedundantAttributes: true,
        removeEmptyAttributes: true,
        removeOptionalTags: true
    });

    // Then, find and minimize HTML content within JavaScript strings
    minimized = minimized.replace(/(`|"|')([\s\S]*?)\1/g, (match, quote, content) => {
        if (content.trim().startsWith('<') && content.trim().endsWith('>')) {
            // This looks like HTML content within a string, so let's minimize it
            const minimizedContent = minify(content, {
                removeComments: true,
                collapseWhitespace: true,
                conservativeCollapse: false,
                collapseInlineTagWhitespace: true,
                removeAttributeQuotes: true,
                removeRedundantAttributes: true,
                removeEmptyAttributes: true,
                removeOptionalTags: true
            });
            return quote + minimizedContent + quote;
        }
        return match;
    });

    return minimized;
}

function escapeQuotes(text) {
    return text.replace(/"/g, '\\"');
}

function htmlToCString(htmlContent) {
    const escaped = escapeQuotes(htmlContent).replace(/\n/g, '\\n');
    const lines = [];
    let currentLine = '';

    for (let i = 0; i < escaped.length; i++) {
        currentLine += escaped[i];
        if (currentLine.length >= 80 && escaped[i] !== '\\') {
            lines.push(currentLine);
            currentLine = '';
        }
    }
    if (currentLine) lines.push(currentLine);

    return lines.map((line, index) => {
        // Escape the first quote of a line if it's not already escaped
        if (index > 0 && line.startsWith('"') && !line.startsWith('\\"')) {
            line = '\\' + line;
        }
        // Ensure the line ends with an escaped quote if it ends with a backslash
        if (line.endsWith('\\')) {
            line += '"';
        }
        return `"${line}"`;
    }).join('\n');
}

function main() {
    const inputFile = path.join(__dirname, 'index.html');
    const outputFile = path.join(__dirname, 'index_html.h');

    fs.readFile(inputFile, 'utf8', (err, htmlContent) => {
        if (err) {
            console.error('Error reading input file:', err);
            return;
        }

        const minimizedHtml = minimizeHtml(htmlContent);
        const cString = htmlToCString(minimizedHtml);

        const headerContent = `#ifndef INDEX_HTML_H
#define INDEX_HTML_H

const char INDEX_HTML[] PROGMEM = 
${cString};

#endif // INDEX_HTML_H`;

        fs.writeFile(outputFile, headerContent, 'utf8', (err) => {
            if (err) {
                console.error('Error writing output file:', err);
                return;
            }
            console.log('Successfully created index_html.h');
        });
    });
}

main();
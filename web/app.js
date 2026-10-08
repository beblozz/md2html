const e = React.createElement;

function readTextFile(file, onDone) {
    const reader = new FileReader();
    reader.onload = function () {
        onDone(reader.result);
    };
    reader.readAsText(file);
}

function getBody(html) {
    const doc = new DOMParser().parseFromString(html, "text/html");
    return doc.body.innerHTML;
}

function FileButton(props) {
    function handleChange(event) {
        const file = event.target.files[0];
        if (!file) {
            return;
        }
        readTextFile(file, function (text) {
            props.onLoad(file.name, text);
        });
    }

    return e(
        "label",
        { className: "file-button" },
        props.label,
        e("input", { type: "file", accept: props.accept, onChange: handleChange })
    );
}

function Panel(props) {
    return e(
        "section",
        { className: "panel" },
        e(
            "div",
            { className: "panel-header" },
            e("h2", null, props.title),
            e("span", { className: "file-name" }, props.fileName)
        ),
        props.children
    );
}

function App() {
    const [mdName, setMdName] = React.useState("файл не выбран");
    const [mdText, setMdText] = React.useState("");
    const [htmlName, setHtmlName] = React.useState("файл не выбран");
    const [htmlText, setHtmlText] = React.useState("");
    const [tab, setTab] = React.useState("preview");

    function loadMd(name, text) {
        setMdName(name);
        setMdText(text);
    }

    function loadHtml(name, text) {
        setHtmlName(name);
        setHtmlText(text);
    }

    let result;
    if (htmlText === "") {
        result = e("p", { className: "empty" }, "Выбери .html файл, который сделала программа md2html");
    } else if (tab === "preview") {
        result = e("div", {
            className: "markdown-body",
            dangerouslySetInnerHTML: { __html: getBody(htmlText) }
        });
    } else {
        result = e("pre", { className: "source" }, htmlText);
    }

    let source;
    if (mdText === "") {
        source = e("p", { className: "empty" }, "Выбери исходный .md файл");
    } else {
        source = e("pre", { className: "source" }, mdText);
    }

    return e(
        "div",
        { className: "app" },
        e(
            "header",
            { className: "top" },
            e("h1", null, "md2html viewer"),
            e("p", null, "Слева исходный Markdown, справа HTML, который получился после md2html"),
            e(
                "div",
                { className: "buttons" },
                e(FileButton, { label: "Открыть .md", accept: ".md,.txt", onLoad: loadMd }),
                e(FileButton, { label: "Открыть .html", accept: ".html,.htm", onLoad: loadHtml }),
                e(
                    "button",
                    {
                        className: tab === "preview" ? "tab active" : "tab",
                        onClick: function () { setTab("preview"); }
                    },
                    "Просмотр"
                ),
                e(
                    "button",
                    {
                        className: tab === "code" ? "tab active" : "tab",
                        onClick: function () { setTab("code"); }
                    },
                    "HTML-код"
                )
            )
        ),
        e(
            "div",
            { className: "panels" },
            e(Panel, { title: "Markdown", fileName: mdName }, source),
            e(Panel, { title: "Результат", fileName: htmlName }, result)
        )
    );
}

const root = ReactDOM.createRoot(document.getElementById("root"));
root.render(e(App));

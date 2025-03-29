document.addEventListener("DOMContentLoaded", () => {
  const containers = document.querySelectorAll(".data");

  containers.forEach((container) => {
    const codeElement = container.querySelector(".code-content");
    if (!codeElement) return;

    // 获取代码内容
    let codeContent = codeElement.textContent.trim();

    // 动态转义特殊字符
    const escapedContent = codeContent
      .replace(/</g, "&lt;")
      .replace(/>/g, "&gt;");

    // 包裹代码内容并添加语法高亮类
    codeElement.innerHTML = `<pre><code class="language-c">${escapedContent}</code></pre>`;

    // 触发 Prism.js 的语法高亮
    Prism.highlightElement(codeElement.querySelector("code"));
  });
});
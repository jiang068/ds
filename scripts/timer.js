document.addEventListener("DOMContentLoaded", function () {
    const timesElement = document.getElementById("times");
    if (!timesElement) return;

    const grt = new Date("03/28/2025 20:11:45"); // 网站启用时间

    function createtime() {
        const now = new Date();
        const diff = now - grt;

        const days = Math.floor(diff / (1000 * 60 * 60 * 24));
        const hours = Math.floor((diff / (1000 * 60 * 60)) % 24).toString().padStart(2, "0");
        const minutes = Math.floor((diff / (1000 * 60)) % 60).toString().padStart(2, "0");
        const seconds = Math.floor((diff / 1000) % 60).toString().padStart(2, "0");

        timesElement.textContent = `已运行 ${days} 天 ${hours} 小时 ${minutes} 分 ${seconds} 秒`;
    }

    setInterval(createtime, 250);
});
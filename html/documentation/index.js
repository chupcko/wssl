/* Left navigation: search filter, active link highlight, mobile toggle.
   Entry point initDocumentation() runs from <body onload>. */

function initDocumentation() {
  const search = document.getElementById("search");
  const links = [...document.querySelectorAll(".nav-group a")];
  const toggle = document.getElementById("menu-toggle");
  const sidebar = document.getElementById("sidebar");

  const fillSpots = (className, value) => {
    document.querySelectorAll(`.${className}`).forEach((element) => {
      element.textContent = value;
    });
  };

  if (typeof WSSL_DOCUMENTATION_VERSION !== "undefined") {
    const versionSpot = document.getElementById("doc-version");
    if (versionSpot) {
      versionSpot.textContent = WSSL_DOCUMENTATION_VERSION;
    }
    fillSpots("doc-version", WSSL_DOCUMENTATION_VERSION);
  }
  if (typeof WSSL_DOCUMENTATION_SERVER_NAME !== "undefined") {
    fillSpots("doc-server-name", WSSL_DOCUMENTATION_SERVER_NAME);
  }

  toggle?.addEventListener("click", () => sidebar?.classList.toggle("open"));
  sidebar?.addEventListener("click", (event) => {
    if (event.target.tagName === "A") {
      sidebar.classList.remove("open");
    }
  });

  search?.addEventListener("input", () => {
    const query = search.value.toLowerCase();
    links.forEach((link) => {
      link.parentElement.style.display = link.textContent.toLowerCase().includes(query) ? "" : "none";
    });
    document.querySelectorAll(".nav-group").forEach((group) => {
      const visible = group.querySelectorAll("li:not([style*='none'])").length;
      group.style.display = visible > 0 ? "" : "none";
    });
  });

  if (!("IntersectionObserver" in window)) {
    return;
  }

  const map = {};
  links.forEach((link) => {
    map[link.getAttribute("href").slice(1)] = link;
  });

  const observer = new IntersectionObserver((entries) => {
    entries.forEach((entry) => {
      if (entry.isIntersecting) {
        links.forEach((link) => link.classList.remove("active"));
        map[entry.target.id]?.classList.add("active");
      }
    });
  }, { rootMargin: "-20% 0px -70% 0px" });

  document.querySelectorAll(".content section[id]").forEach((section) => observer.observe(section));
}

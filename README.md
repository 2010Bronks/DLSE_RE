# 🏰 Dungeon Lords Steam Edition Reverse-Engineered (DLSE_RE)

**DLSE_RE** is a reverse-engineering project for *Dungeon Lords Steam Edition*, created with the ultimate goal of enabling full modding support for the game.

While the original vision was to build a complete **MOD CORE + MOD API**, the current implementation is still far from that milestone. At this stage, you can think of this project primarily as a **Raw SDK** for the game.

---

## 📖 About the Project

This project is entirely based on the reverse engineering of the game client. Some of the information regarding the *Granny* library was sourced from publicly available open references.

> **🔐 IDA Pro Project File**  
> If you need the IDA Project File (`.i64`), please reach out to me directly.  
> *Note that you will need **IDA Pro 9.1** to open it.*

---

## ⚙️ What Can You Already Do with the Game?

Although the current functionality is still limited, the existing systems already allow you to:

- **Adjust Adaptive Difficulty** – Dynamically modify enemy stats, loot tables, drop chances, and mob spawn counts.
- **Edit Your Character** – Tweak player stats, inventory, skills, heraldries, magic, and item properties in real-time.

---

## 🚀 Features Available in the Project

The following features have already been implemented:

- **Mouse Fix** – Corrects mouse behavior when playing in windowed mode (use the `-gdi` startup parameter).
- **OOB Crash Fix** – Resolves the crash that occurs when exiting Out Of Bounds (OOB) areas.
- **Memory & Hooking System** – A convenient framework for obtaining pointers, hooking functions, and managing game memory (powered by the `memoria dep` module).
- **God Mode** – Includes simple invincibility, reflect damage, and a mode where no damage is dealt to anyone.
- **Infinite Durability** – Items no longer lose durability over time.
- **Reduced Spell Cooldowns** – Shorten the cooldown period for spells.
- **Teleportation** – Teleport your character (which can also be used to clip through walls).
- *...etc*

---

## 🤝 Contributing

Feel free to explore the code, open issues, or submit pull requests. Since this is a raw SDK at the moment, any help with research, documentation, or feature implementation is highly appreciated!

---

## 📜 License

This project is licensed under the **GNU General Public License v3.0** – see the [LICENSE](LICENSE) file for details.
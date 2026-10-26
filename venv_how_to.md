# 1) ставим venv-поддержку — С sudo
sudo apt install -y python3.12-venv python3-full

# 2) удаляем неудачный (пустой) venv
cd ~/leti_labs/4\ KURS/MNDS
rm -rf venv

# 3) создаём заново
python3 -m venv venv

# 4) активируем
source venv/bin/activate

# 5) в приглашении появится (venv) — ставим пакеты
pip install --upgrade pip
pip install numpy matplotlib tqdm
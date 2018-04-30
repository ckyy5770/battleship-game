import subprocess
from pathlib import Path

p_root = Path.cwd()

subprocess.run(args=['git', 'add', '--all'], cwd=p_root)
subprocess.run(args=['git', 'commit', '-m', 'update'], cwd=p_root)
subprocess.run(args=['git', 'push'], cwd=p_root)

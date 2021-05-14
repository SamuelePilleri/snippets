## Setup

First setup a virtual environment and download dependencies.

```sh
python -m venv venv
source venv/bin/activate
pip install -r requirements.txt
```

Or, for Windows:

```batch
python -m venv venv
venv\Scripts\activate.bat
pip install -r requirements.txt
```

Then obtain your API keys from <https://my.telegram.org> and save it to a `config.ini` file as follows:

```ini
[Telegram]
API_ID = 1234567
API_HASH = abcdef1234567890abcdef1234567890
CHAT_ID = me  # or your favourite chat ID to send files to
```

**Don't share your keys with anyone!**

The first time you run the script it will require to log-in to your Telegram account using a code provided in-app.

## Run

Edit the `jobs.yaml` file according to your needs:

```yaml
jobs:
 - url: http://file/to/download
   filename: optional_filename.ext
   caption: optional caption
 - url: http://file/to/download
   filename: optional_filename.ext
   caption: optional caption
...
```

Run the script with `python main.py`

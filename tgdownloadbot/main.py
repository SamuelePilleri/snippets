import requests
import yaml

from calmsize import size, alternative
from configparser import ConfigParser
from datetime import datetime
from os.path import basename
from telethon import TelegramClient
from urllib.parse import urlparse


async def download_and_send(self: TelegramClient, chat_id: str, url: str, filename: str, caption: str):
    progress = 0
    timestamp = datetime.now()
    def speed_callback(sent, total):
        nonlocal progress, timestamp
        new_ts = datetime.now()
        speed = (sent - progress) / ((new_ts.microsecond - timestamp.microsecond) / 1e6)
        if speed > 0:
            print("\033[KTransferring {:.1f}/{:.1f} of {} @ {:.1f}/s"
                .format(size(sent), size(total), filename, size(speed, system=alternative)),
                end='\r'
            )
        progress = sent
        timestamp = new_ts

    with requests.get(url, stream=True) as r:
        # r.raw.decode_content = True # https://stackoverflow.com/a/60846477 (comments)
        tgfile = await self.upload_file(
            r.raw,
            file_name=filename,
            file_size=int(r.headers['Content-length']),
            progress_callback=speed_callback
        )
        await self.send_file(chat_id, tgfile, caption=caption)


TelegramClient.download_and_send = download_and_send


def main():
    config = ConfigParser()
    config.read("config.ini")
    api_id = config["Telegram"]["API_ID"]
    api_hash = config["Telegram"]["API_HASH"]
    chat_id = config["Telegram"]["CHAT_ID"]

    client = TelegramClient('app', api_id, api_hash)
    client.start()

    with open("jobs.yaml") as fp:
        jobs = yaml.safe_load(fp)
        for job in jobs["jobs"]:
            url = job["url"]
            filename = job.get("filename", basename(urlparse(url).path))
            caption = job.get("caption", "")
            client.loop.run_until_complete(client.download_and_send(chat_id, url, filename, caption))


if __name__ == "__main__":
    main()

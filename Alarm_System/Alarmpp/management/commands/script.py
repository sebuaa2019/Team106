from Alarmpp.my_socket import my_socket_server
from django.core.management import BaseCommand

class Command(BaseCommand):

    def handle(self, *args, **options):
        board_server = my_socket_server("127.0.0.1", 12345)
        board_server.start()
        board_server.getsender()


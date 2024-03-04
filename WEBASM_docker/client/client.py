import urllib.request

fp = urllib.request.urlopen("http://localhost:1234/") # Эта переменная содержит запрос к 'http://localhost:1234/'.

encodedContent = fp.read()
decodedContent = encodedContent.decode("utf8")

print(decodedContent)

fp.close()
# GVSApp

①GvsApp/SeeeduinoCCC以下のプログラムを電気刺激装置のマイコンに書き込む（プログラム内の変数g_Chをデバイスごとに書き換え）

②GvsApp/build/GVSApp/GVS.exeを実行．「Add Device」を押し，表示されたボックス内のIDを1.で書き換えたg_Chに合わせる．また，PortNameもデバイスが接続されているポートに合わせて書き換える．Current,Frequency,Duration,WaveFormをそれぞれ調整したらStartで刺激開始．

注意点
・ポート名が間違っている場合はその旨が表示されるので正しいポート名に直す．
・ポート名が合っていてもデバイスからの応答がない場合，ボックスの左下に「No device」と表示される．この場合はTeratarm等を駆使してデバッグする．
・予期せぬバグによって電流が止まらなくなる可能性は十分にありうるので，そうなったらデバイスの給電を抜いて強制的に停止させる．

---
#macで使いたい場合
Arduino IDEでSeeduinoCCC以下のプログラムを書くところまではWindows(上記)と同様で大丈夫だけど，QtCreatorのバージョンを揃えたりPortNameの調べ方がちょっと癖あるので注意する．
事前準備として，Xcodeをダウンロードしておくこと．QtCreatorも大概だけどこちらもめちゃ重いので時間がかかる．

SeeduinoXIAOに書き込む方法がわからない時は[公式](https://wiki.seeedstudio.com/jp/Seeeduino-XIAO/)を参考にする．

##以下手順
1. QtCreatorのインストローラを[HP](https://www.qt.io/download-open-source)からダウンロードする
2. Installするverを選択する時に，Archiveにチェックを入れてFilterをかける
3. ***Qt/5.12.10***にチェックを入れて付属のモジュールもとりあえず全てチェックしてインストール(時間かかる)
4. GVS.proをダブルクリックしてQtCreatorで開く
5. configurationしてプロジェクトをインポートする
6. ターミナルを開いて，`ls -l /dev/tty.*`のコマンドを実行してUSBのポート番号を調べる
7. `tty.usbserial-hoge`をコピーしておく
8. GVS.appを左下の再生マークから実行する
9. `Add Devices`からChannelを指定し，PortNameに先程の`tty.usbserial-hoge`を入力
10. Startボタンを押して実行

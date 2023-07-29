# InitializeForgotEngine

## 2D/3D描画エンジン

  DirectX12を用いた自作エンジンです。  
  コンポーネント指向を組み込みエディタとしても利用できます。

### 基本機能
- コンポーネント
- サウンドの読み込み(waveのみの対応)
- シーンデータの書き込み及び読み込み

### エディタ機能
  起動中にImgui上で行うことができる操作
- モデル読み込み
- テクスチャ読み込み
- オブジェクトの追加
- スプライトの追加
- パーティクルの追加
- 色の変更
- マテリアルの変更
- テクスチャの変更
- コライダーの追加
など

## 推奨環境
- windows10

## 使用外部ライブラリ
- [Assimp](https://github.com/assimp/assimp)
  - モデルの読み込みに使用
- [DirectXTex](https://github.com/microsoft/DirectXTex)
  - テクスチャの読み込みに使用
- [Dear ImGui](https://github.com/ocornut/imgui)
  - エディタ画面のGUIに使用
- [JSON for Modern C++](https://github.com/nlohmann/json)
  - シーンデータの書き込み及び読み込みに使用

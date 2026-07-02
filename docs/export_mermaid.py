"""
租书店管理系统 - Mermaid 流程图导出为高清 PNG
"""
import os
import sys
import subprocess

if sys.platform == 'win32':
    sys.stdout.reconfigure(encoding='utf-8')

OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "..", "docs", "images")
os.makedirs(OUTPUT_DIR, exist_ok=True)

# ========== Mermaid 流程图定义 ==========

diagrams = {}

diagrams["01_系统登录流程"] = """flowchart TD
    A([启动系统]) --> B[选择角色<br/>管理员/店员/会员]
    B --> C[输入账号密码]
    C --> D[读取 user.csv 验证]
    D --> E{验证通过?}
    E -->|是| F[显示角色菜单]
    E -->|否| C
    F --> G([进入系统])
"""

diagrams["02_图书租借流程"] = """flowchart TD
    A([开始]) --> B[输入会员账号和图书ID]
    B --> C{会员存在?}
    C -->|是| D[验证图书库存]
    C -->|否| Z1([提示:会员不存在])
    D --> E{库存>0?}
    E -->|是| F[生成租借记录<br/>ID=R+时间戳]
    E -->|否| Z2([提示:库存不足])
    F --> G[库存 -1]
    G --> H[计算应还日期<br/>租借日+30天]
    H --> I[保存到 rental.csv]
    I --> J([结束])
    Z1 --> J
    Z2 --> J
"""

diagrams["03_图书归还流程"] = """flowchart TD
    A([开始]) --> B[输入租借记录ID]
    B --> C[查找租借记录]
    C --> D{记录存在<br/>且未归还?}
    D -->|是| E[设置归还日期=当前日期]
    D -->|否| Z([提示:记录无效])
    E --> F[库存 +1]
    F --> G[计算租借费用<br/>书价×1%×天数]
    G --> H{逾期>30天?}
    H -->|是| I[计算逾期罚款<br/>书价×2%×逾期天数]
    H -->|否| J[显示费用明细]
    I --> J
    J --> K[更新 rental.csv]
    K --> L([结束])
    Z --> L
"""

diagrams["04_会员注册流程"] = """flowchart TD
    A([开始]) --> B[管理员选择添加会员]
    B --> C[输入会员信息<br/>账号/密码/姓名/手机]
    C --> D{账号重复?}
    D -->|是| C
    D -->|否| E[写入 member.csv]
    E --> F[自动生成登录账号<br/>role=3]
    F --> G[写入 user.csv]
    G --> H([结束])
"""

diagrams["05_系统架构总览"] = """flowchart TB
    subgraph Presentation["表现层 main.cpp"]
        A1[菜单显示]
        A2[登录验证]
        A3[输入处理]
        A4[角色路由]
    end
    
    subgraph Logic["业务逻辑层"]
        B1[BookManager<br/>图书CRUD]
        B2[MemberManager<br/>会员CRUD]
        B3[RentalManager<br/>租借/归还]
        B4[DateUtil<br/>日期计算]
    end
    
    subgraph Data["数据层 CSV"]
        C1[user.csv]
        C2[book.csv]
        C3[category.csv]
        C4[member.csv]
        C5[rental.csv]
    end
    
    A1 --> B1
    A1 --> B2
    A2 --> B2
    A3 --> B3
    A4 --> B1
    A4 --> B2
    A4 --> B3
    
    B1 --> C2
    B1 --> C3
    B2 --> C1
    B2 --> C4
    B3 --> C5
    B4 --> C5
"""

diagrams["06_综合项目主流程"] = """flowchart TD
    A([启动系统]) --> B[加载CSV数据到内存]
    B --> C[选择角色并登录]
    C --> D{验证通过?}
    D -->|否| C
    D -->|是| E{角色?}
    
    E -->|管理员| F
    E -->|店员| G
    E -->|会员| H
    
    subgraph Admin ["管理员功能"]
        F1[图书管理<br/>增删改查]
        F2[会员管理<br/>增删改查]
        F3[代会员租借/归还]
        F4[查看逾期记录]
        F5[保存数据到CSV]
    end
    
    subgraph Staff ["店员功能"]
        G1[代会员租借图书]
        G2[归还图书]
        G3[查看逾期记录]
    end
    
    subgraph Member ["会员功能"]
        H1[查看图书列表]
        H2[自行租借图书]
        H3[归还图书]
        H4[查看自己的租借记录]
    end
    
    F --> F1 --> F2 --> F3 --> F4 --> F5
    G --> G1 --> G2 --> G3
    H --> H1 --> H2 --> H3 --> H4
    
    F5 --> I{继续操作?}
    G3 --> I
    H4 --> I
    I -->|是| E
    I -->|否| J([退出系统])
"""

# ========== 执行导出 ==========
if __name__ == "__main__":
    print("=" * 50)
    print("租书店管理系统 - Mermaid 流程图导出")
    print("=" * 50)
    
    # mmdc 选项：高清 PNG，带透明背景
    opts = ["--backgroundColor", "#FFFFFF", "--scale", "2"]
    
    success = 0
    fail = 0
    
    for name, mermaid_code in diagrams.items():
        print(f"\n渲染: {name}")
        md_file = os.path.join(OUTPUT_DIR, f"{name}.mmd")
        png_file = os.path.join(OUTPUT_DIR, f"{name}.png")
        
        # 写 .mmd 文件
        with open(md_file, 'w', encoding='utf-8') as f:
            f.write(mermaid_code)
        
        # 用 mmdc 渲染为 PNG
        try:
            mmdc_cmd = "C:\\Users\\LeGo\\AppData\\Roaming\\npm\\mmdc.ps1"
            result = subprocess.run(
                ["pwsh", "-ExecutionPolicy", "Bypass", "-File", mmdc_cmd, "-i", md_file, "-o", png_file] + opts,
                capture_output=True, text=True, timeout=60
            )
            if os.path.exists(png_file) and os.path.getsize(png_file) > 1000:
                size = os.path.getsize(png_file)
                print(f"  OK -> {png_file} ({size} bytes)")
                success += 1
            else:
                print(f"  FAIL -> {result.stderr[:200] if result.stderr else 'empty output'}")
                fail += 1
        except subprocess.TimeoutExpired:
            print(f"  FAIL -> timeout")
            fail += 1
        except Exception as e:
            print(f"  FAIL -> {e}")
            fail += 1
    
    print("\n" + "=" * 50)
    print(f"完成! 成功: {success}, 失败: {fail}")
    print(f"图片目录: {os.path.abspath(OUTPUT_DIR)}")
    print("=" * 50)

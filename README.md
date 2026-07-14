# UAV-Design-Schematic-Engine
An interactive aerospace engineering dashboard designed for live UAV layout tracking.Moving input sliders triggers realtime aerodynamic calculations for main wing/tail surface sizing flight performance metrics (Reynolds number stall/cruise speeds), and fluid forces (lift and drag parameters) while dynamically shifting the blueprint vector graphics.
 **Note:** **This is a full-stack local application. The interactive calculations output and vector layout schematic UAV design simulation on Canvas  require running `bridge.py` locally (which connects to the compiled C++ engine). See [Setup Instructions](#setup) below to run it fully.**
 ## How to Run the Application Locally
 * **Since this engineering dashboard processes aerospace equations through a high-performance desktop pipeline, it runs locally on your machine using a C++ backend and a Python data bridge.
 * open the VS code
  # ## step by step execution:
 * **Download the Repository:**  **Download ZIP** (or clone the repository). Extract the files(the UAV Engine $ UAV Frontend $ bridge.py) to a folder.
 * **Compile the C++ Engine:** Open your terminal/command prompt in the project directory and compile the backend core logic:
 * **g++ UAV_workspace_Engine.cpp -o UAV_workspace_Engine.exe**
  * **python bridge.py**
  * # ## video attached for same
  * https://github.com/user-attachments/assets/d71a7a83-13f9-4d6f-8cb0-17d3d1c20b45
  * # ##Demo video of Web
   https://github.com/user-attachments/assets/a32dc96e-ce29-45f0-94f1-f481270a47ed
# Interactive UAV Design & Layout Dashboard
# Dynamic Control Vectors (Interactive Sliders)
*The dashboard features real-time control inputs mapped via JavaScript event hooks:
* **Wing Parameters:** Wing Loading  & Main Wing Aspect Ratio and Aspect Ratios of Horizontal and Vertical Tails as well as there Tail Volume Coefficients. 
* **Flight Envelope:** Wind Tunnel Airspeed & Operating Angle of Attack .
* **Aerodynamic Constraints:** Maximum Lift Coefficient.
* **Mass & Balance:** Center of Gravity Payload Balance Shifts.
# Live Calculated Outputs
Adjusting the sliders triggers instant mathematical updates across the schematic matrix:
* **Planform Sizing:** Main Wing area, span, and Mean Aerodynamic Chord (MAC).
* **Stability Indicators:** Dynamic parsing of Horizontal and Vertical  aircraft stabilizer arrays (chord tips, span grids, and elevator breadth).
* **Fluid Forces:** Real-time optimization updates for dynamic Lift , Drag forces ,cl , and cd profiles evaluated against a constant kinematic air viscosity (nu \approx 1.5 \times 10^{-5} ).
# Tech Stack & Architecture
* **Aerodynamics Engine & Bridge:** Integrated C++ computational workspace modules (`UAV_workspace_Engine.cpp`) bridged seamlessly via structural Python scripting logic (`bridge.py`).
* **Frontend UI Framework:** HTML5 Custom Graphic Components, CSS Grid Matrices.
* **Core Execution Pipeline:** (JavaScript) Event Listeners for real-time state management and dynamic canvas updates.
**Dynamic Transformations:** Watch the schematic vector layout scale instantly as you move control inputs.
***Observe** the calculation matrix update flight metrics and also an Aerodynamic Pop up Guide tells how sliders extremes affect Vector layouts and outputs calculation.
# DEVELOPMENT NOTE : 
* **Core Engineering Work:** UAV_workspace_Engine.cpp — The full aerodynamic and structural sizing engine (wing planform sizing, tail volume coefficient calculations, stall/cruise velocity, Reynolds number, lift/drag computation, elevator and rudder sizing) was independently designed and implemented by me. All governing equations, constants, and engineering assumptions reflect my own aerospace calculations.
* **Frontend UI/Layout (UAV frontend.html + CSS)** — The interactive dashboard layout, card-based slider system (Card A: Sizing Constraints, Card B: Environment & Balance), the live canvas schematic (top-down wing view, V-tail side profile, dimension-line annotations), and the aerodynamic guide modal were designed and built by me.
* **AI ASSISTANCE USED IN Building the Bridge.py and function runaerodynamicpipeline()(in UAV FRontend.html): were used to help scaffold the glue layer connecting the two — the Python bridge server and the JS data-fetching pipeline**
